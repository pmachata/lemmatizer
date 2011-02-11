#include <cstring>
#include <iconv.h>
#include <iostream>
#include <stdexcept>

#include <AgramtabLib/RusGramTab.h>

#include <cs/cs.h>
#include <boost/filesystem.hpp>

#include "forms.hh"
#include "lemmatize.hh"
#include "part_of_speech.hh"
#include "pos_handler.hh"
#include "backend.hh"
#include "fcgi_backend.hh"
#include "rus_gramtab.hh"
#include "template_cache.hh"
#include "format.hh"

void
fail (std::string const &reason)
{
  throw std::runtime_error (reason + ": " + std::strerror (errno) + ".");
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
    boost::filesystem::path cwd (boost::filesystem::current_path ());
    handle_neoerr (hdf_set_valuef (*this, "hdf.loadpaths.0=%s",
				   cwd.string ().c_str ()));
  }
};

// Use this character as an accent mark.  This must be a wchar_t
// constant.  In non-unicode environment, this should be set to L"'"
// or some such.
//#define ACCENT_MARK L"\u0301"
#define ACCENT_MARK "\xcc\x81" // this is UTF-8

class lemmatizer_app
{
  CLemmatizer *_m_lemmatizer;
  CAgramtab *_m_agramtab;
  iconv_t _m_iconv_to;
  iconv_t _m_iconv_from;
  pos_handler_map _m_handlers;
  template_cache _m_templates;
  default_hdf _m_hdf;

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

      // Just assume utf-8.  That's ubiquitous on Linuxes and
      // apparently encoding URLs in utf-8 is standard, too.

    , _m_iconv_to (check (iconv_open ("windows-1251", "utf-8"),
			  "iconv_open to"))

    , _m_iconv_from (check (iconv_open ("utf-8", "windows-1251"),
			    "iconv_open from"))
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
    if (accent == -1)
      return convert (w, _m_iconv_from);

    std::string s = convert (w.substr (0, accent + 1), _m_iconv_from);
    s += ACCENT_MARK;
    s += convert (w.substr (accent + 1), _m_iconv_from);

    // XXX here we would really like to convert the string to lower case
    return s;
  }

  void
  process (std::string const &line, backend *back)
  {
    lemmatize lem (convert (line, _m_iconv_to), _m_lemmatizer, _m_agramtab);
    for (lemmatize::const_iterator it = lem.begin ();
	 it != lem.end (); ++it)
      {
	part_of_speech pos = it.get_part_of_speech ();
	lemmatize::forms forms = it.forms ();

	pos_handler const *handler
	  = _m_handlers.get_handler (pos.number ());
	if (handler == NULL)
	  {
	    std::cerr << "No handler for part of speech \""
		      << show (pos.name ()) << "\" ("
		      << pos.number () << ")." << std::endl;
	    continue;
	  }

	CSPARSE *tmpl = _m_templates.get (pos.number ());
	if (tmpl == NULL)
	  {
	    std::string file_name = handler->template_name ();
	    file_name += ".cs";
	    try
	      {
		tmpl = _m_templates.add (_m_hdf, pos.number (),
					 file_name.c_str ());
		if (tmpl == NULL)
		  throw std::runtime_error ("Unknown reason.");
	      }
	    catch (std::runtime_error const &err)
	      {
		std::cerr << "Couldn't load the template \""
			  << file_name << ": " << err.what ()
			  << "." << std::endl;
		continue;
	      }
	    assert (tmpl != NULL);
	  }
	assert (tmpl != NULL);

	hdf_data_map data;
	handler->fill_hdf (_m_agramtab, it, data);

	data["pos"].push_back
	  (std::make_pair (format_rus (pos.number_as<pos_code_t> ()), -1));

	for (hdf_data_map::const_iterator it = data.begin ();
	     it != data.end (); ++it)
	  {
	    std::string name = std::string ("Form.") + show (it->first);
	    HDF *node;
	    handle_neoerr (hdf_get_node (_m_hdf, name.c_str (), &node));
	    for (size_t i = 0; i < it->second.size (); ++i)
	      {
		std::string const &word = it->second[i].first;
		int accent = it->second[i].second;
		std::string final = show (word, accent);
		handle_neoerr (hdf_set_valuef (node, "%zd=%s",
					       i, final.c_str ()));
	      }
	  }

	handle_neoerr (hdf_dump (_m_hdf, ">"));

	struct _ {
	  static NEOERR *render_cb (void *data, char *str)
	  {
	    backend *b = (backend *)data;
	    bool result = b->render (str);

	    if (result)
	      return NULL;
	    else
	      return nerr_raise (NERR_PARSE, "Backend render failed.");
	  }
	};

	handle_neoerr (cs_render (tmpl, back, &_::render_cb));
	handle_neoerr (hdf_remove_tree (_m_hdf, "Form"));
      }
  }

  int
  loop (backend *back)
  {
    std::string line;
    while (back->get_word (line))
      {
	back->before_render ();
	Trim (line);
	if (line.empty ())
	  continue;

	// XXX here we would perhaps like to convert the string to
	// lower case.

	process (line, back);
	back->after_render ();
      }
    return 0;
  }

  int
  run ()
  {
    console_backend back (std::cin, std::cout);
    return loop (&back);
  }

  int
  fcgi_run ()
  {
    fcgi_backend back;
    return loop (&back);
  }
};

int main(int argc, char **argv)
  try
    {
      setlocale (LC_ALL, "");
      std::cerr << "LC_ALL=" << setlocale (LC_ALL, NULL) << std::endl;
      return lemmatizer_app ().fcgi_run ();
    }
  catch (CExpc const &exc)
    {
      std::cerr << "seman exception: " << exc.m_strCause << std::endl;
      return 1;
    }
  catch (std::runtime_error const &err)
    {
      std::cerr << "runtime error: " << err.what () << std::endl;
      return 1;
    }
