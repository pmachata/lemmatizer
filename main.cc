#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iconv.h>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include <AgramtabLib/RusGramTab.h>

#include <cs/cs.h>
#include <util/neo_files.h>

#include "lemmatize.hh"
#include "gramcodes.hh"
#include "forms.hh"
#include "part_of_speech.hh"
#include "config.hh"
#include "rus_gramtab.hh"
#include "pos_handler.hh"
#include "noun.hh"
#include "adjective.hh"

void
fail (std::string const &reason)
{
  throw std::runtime_error (reason + ": " + std::strerror (errno) + ".");
}

// Convert multi-byte string to wide string.
std::wstring
to_w (std::string s)
{
  wchar_t buf[s.length () + 1];
  char const *src = s.c_str ();
  size_t len = mbsrtowcs(buf, &src, s.length (), NULL);
  if (len == (size_t)-1)
    fail ("mbsrtowcs");
  buf[len] = 0;
  return buf;
}

// Convert wide string to multi-byte string.
std::string
to_mb (std::wstring s)
{
  // ask how much we need
  wchar_t const *src = s.c_str ();
  size_t len = wcstombs(NULL, src, 0);
  if (len == (size_t)-1)
    fail ("wcstombs");

  char buf[len + 1];
  wcstombs(buf, src, len);
  buf[len] = 0;
  return buf;
}

// Convert wide string to lower-case.
void
to_lower (std::wstring &word)
{
  std::transform (word.begin (), word.end (), word.begin (), towlower);
}

// iconv convenience wrapper.
std::string
convert (std::string const &word, iconv_t ic_d)
{
  char *inbuf = const_cast<char *>(word.c_str ());
  size_t inbuf_size = strlen (inbuf);
  size_t inbuf_left = inbuf_size;
  size_t outbuf_size = inbuf_size * 2 + 1; // Russian takes two bytes at most
  size_t outbuf_left = outbuf_size;
  char outbuf[outbuf_size];
  char *outbuf_ptr = outbuf;

  if (iconv (ic_d, NULL, NULL, NULL, NULL) == (size_t)-1
      || iconv (ic_d, &inbuf, &inbuf_left,
		&outbuf_ptr, &outbuf_left) == (size_t)-1)
    fail ("iconv");

  *outbuf_ptr = 0;

  return outbuf;
}

void
handle_neoerr (NEOERR *err)
{
  if (err != NULL)
    {
      STRING str;
      string_init (&str);
      nerr_error_string (err, &str);
      std::string what = str.buf;
      string_clear (&str);
      throw std::runtime_error (what);
    }
}

class hdf
{
  HDF *_m_hdf;

public:
  hdf ()
  {
    handle_neoerr (hdf_init (&_m_hdf));
  }

  ~hdf ()
  {
    hdf_destroy (&_m_hdf);
  }

  operator HDF* ()
  {
    return _m_hdf;
  }
};

class default_hdf
  : public hdf
{
public:
  default_hdf ()
  {
    handle_neoerr (hdf_set_valuef (*this, "hdf.loadpaths.0=%s",
				   config::path.c_str ()));
  }
};

class template_cache
  : public std::vector<CSPARSE *>
{
  typedef std::vector<CSPARSE *> super_t;
public:
  CSPARSE *add (HDF *hdf, int id, char const *template_name)
  {
    assert (id >= 0);
    if ((size_t)id >= size ())
      resize ((size_t)id + 1);
    assert (at (id) == NULL);

    CSPARSE *parse;
    handle_neoerr (cs_init (&parse, hdf));
    handle_neoerr (cs_parse_file (parse, template_name));
    (*this)[id] = parse;

    return get (id);
  }

  CSPARSE *get (int id)
  {
    if (id < 0 || (size_t)id >= size ())
      return NULL;
    else
      return (*this)[id];
  }

  ~template_cache ()
  {
    for (const_iterator it = begin (); it != end (); ++it)
      {
	CSPARSE *parse = *it;
	cs_destroy (&parse);
      }
  }
};

class verb_handler
  : public pos_handler
{
public:
  verb_handler ()
    : pos_handler ("verb")
  {}
};

class pos_handler_map
  : private std::map<int, pos_handler const *>
{
public:
  pos_handler_map ()
  {
    insert (std::make_pair ((int)pos_noun,
			    new noun_handler ()));

    insert (std::make_pair ((int)pos_verb,
			    new verb_handler ()));

    insert (std::make_pair ((int)pos_adjective,
			    new adjective_handler ()));

    insert (std::make_pair ((int)pos_short_adjective,
			    new short_adjective_handler ()));
  }

  ~pos_handler_map ()
  {
    std::set<pos_handler const *> deleted;
    for (iterator it = begin (); it != end (); ++it)
      {
	// Take care not to delete twice.  We update the cache for
	// delegation.
	pos_handler const *to_delete = it->second;
	if (deleted.find (to_delete) == deleted.end ())
	  delete to_delete;
	deleted.insert (to_delete);
      }
  }

  virtual pos_handler const *
  get_handler (int pos)
  {
    int orig_pos = pos;
    int delegated_pos;
    pos_handler const *handler = NULL;
    while (true)
      {
	const_iterator it = find (pos);
	if (it == end ())
	  return NULL;
	handler = it->second;
	delegated_pos = pos;
	pos = handler->delegate ();
	if (pos == -1)
	  break;

	// Guard against infinite cycle.
	if (pos == orig_pos)
	  return NULL;
      }
    assert (handler != NULL);

    // Update the cache so that we don't have to go through delegation
    // loops next time around.
    if (delegated_pos != orig_pos)
      {
	delete (*this)[orig_pos];
	(*this)[orig_pos] = handler;
      }

    return handler;
  }
};

// Use this character as an accent mark.  This must be a wchar_t
// constant.  In non-unicode environment, this should be set to L"'"
// or some such.
#define ACCENT_MARK L"\u0301"

class lemmatizer_app
{
  CLemmatizer *_m_lemmatizer;
  CAgramtab *_m_agramtab;
  iconv_t _m_iconv_to;
  iconv_t _m_iconv_from;
  pos_handler_map _m_handlers;
  template_cache _m_templates;

  template <class T>
  T check (T retval, std::string const &reason)
  {
    if (retval == (T)-1)
      fail (reason);
    return retval;
  }

public:
  lemmatizer_app ()
    : _m_lemmatizer (new CLemmatizerRussian ())
    , _m_agramtab (new CRusGramTab ())
      // local -> windows-1251
    , _m_iconv_to (check (iconv_open ("windows-1251", ""), "iconv_open to"))
      // local <- windows-1251
    , _m_iconv_from (check (iconv_open ("", "windows-1251"), "iconv_open from"))
  {
    string err;
    if (!_m_lemmatizer->LoadDictionariesRegistry(err))
      throw std::runtime_error \
	(std::string ("Can't load morphological dictionary: ") + err + ".");

    if (!_m_agramtab->LoadFromRegistry())
      throw std::runtime_error ("Can't load gramtab.");
  }

  ~lemmatizer_app ()
  {
    delete _m_lemmatizer;
    delete _m_agramtab;
    iconv_close (_m_iconv_to);
    iconv_close (_m_iconv_from);
  }

  // Convert the string to on-screen representation.  If accent is
  // passed, put accent mark where it should be.
  std::string show (std::string const &w, int accent = -1)
  {
    std::string mbw = convert (w, _m_iconv_from);
    std::wstring ww = to_w (mbw);
    to_lower (ww);
    if (accent != -1)
      ww = ww.substr (0, accent + 1) + ACCENT_MARK + ww.substr (accent + 1);
    mbw = to_mb (ww);
    return mbw;
  }

  int run ()
  {
    std::cout << "Content-type: text/html\n\n";

    while (true)
      {
	std::string line;
	std::getline (std::cin, line);
	Trim (line);
	if (line.empty () || std::cin.eof ())
	  break;

	std::wstring wline = to_w (line);
	to_lower (wline);
	line = to_mb (wline);
	std::cout << "convert:  " << line << std::endl;

	lemmatize lem (convert (line, _m_iconv_to), _m_lemmatizer, _m_agramtab);
	for (lemmatize::const_iterator it = lem.begin ();
	     it != lem.end (); ++it)
	  {
	    std::cout << "found:    " << (it.found () ? "yes" : "no")
		      << " (id " << std::hex << it->GetParadigmId () << std::dec << ")" << std::endl;
	    std::cout << "src norm: " << show (it->GetSrcNorm ()) << std::endl;

	    part_of_speech pos = it.get_part_of_speech ();
	    lemmatize::forms forms = it.forms ();

	    struct _
	    {
	      static NEOERR *render_cb (void *data, char *str)
	      {
		std::cout << str;
		return NULL;
	      }
	    };

	    pos_handler const *handler
	      = _m_handlers.get_handler (pos.number ());
	    if (handler == NULL)
	      {
		std::cout << "No handler for part of speech \""
			  << show (pos.name ()) << "\" ("
			  << pos.number () << ")." << std::endl;
		continue;
	      }

	    default_hdf hdf;

	    CSPARSE *tmpl = _m_templates.get (pos.number ());
	    if (tmpl == NULL)
	      {
		std::string file_name = handler->template_name ();
		file_name += ".cs";
		tmpl = _m_templates.add (hdf, pos.number (),
					 file_name.c_str ());
		if (tmpl == NULL)
		  {
		    std::cout << "Couldn't load the template \""
			      << file_name << "\"." << std::endl;
		    continue;
		  }
	      }
	    assert (tmpl != NULL);

	    hdf_data_map data;
	    handler->fill_hdf (_m_agramtab, it, data);

	    for (hdf_data_map::const_iterator it = data.begin ();
		 it != data.end (); ++it)
	      {
		std::string name = std::string ("Form.") + it->first;
		HDF *node;
		handle_neoerr (hdf_get_node (hdf, name.c_str (), &node));
		for (size_t i = 0; i < it->second.size (); ++i)
		  {
		    std::string const &word = it->second[i].first;
		    int accent = it->second[i].second;
		    std::string final = show (word, accent);
		    handle_neoerr (hdf_set_valuef (node, "%zd=%s",
						   i, final.c_str ()));
		  }
	      }

	    handle_neoerr (hdf_dump (hdf, ">"));
	    handle_neoerr (cs_render (tmpl, this, &_::render_cb));
	  }
      }
    return 0;
  }
};

int main(int argc, char **argv)
  try
    {
      setlocale (LC_ALL, "");
      return lemmatizer_app ().run ();
    }
  catch (CExpc const &exc)
    {
      std::cerr << "seman exception: " << exc.m_strCause << std::endl;
      return 1;
    }
