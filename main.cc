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

void fail (std::string const &reason)
{
  throw std::runtime_error (reason + ": " + std::strerror (errno) + ".");
}

// Convert multi-byte string to wide string.
std::wstring to_w (std::string s)
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
std::string to_mb (std::wstring s)
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
void to_lower (std::wstring &word)
{
  std::transform (word.begin (), word.end (), word.begin (), towlower);
}

// iconv convenience wrapper.
std::string convert (std::string const &word, iconv_t ic_d)
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

void handle_neoerr (NEOERR *err)
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

    operator HDF *()
    {
      return _m_hdf;
    }
  };

  class template_cache
    : public std::vector<CSPARSE *>
  {
    typedef std::vector<CSPARSE *> super_t;
  public:
    void push_back (HDF *hdf, std::string const &filename)
    {
      CSPARSE *parse;
      handle_neoerr (cs_init (&parse, hdf));
      handle_neoerr (cs_parse_file (parse, filename.c_str ()));
      super_t::push_back (parse);
    }

    using super_t::push_back;

    ~template_cache ()
    {
      for (const_iterator it = begin (); it != end (); ++it)
	{
	  CSPARSE *parse = *it;
	  cs_destroy (&parse);
	}
    }
  };

  int run ()
  {
    std::cout << "Content-type: text/html\n\n";

    hdf data;
    handle_neoerr (hdf_set_valuef (data, "hdf.loadpaths.0=%s",
				   config::path.c_str ()));

    size_t n = _m_agramtab->GetPartOfSpeechesCount ();
    assert (n < 100); // make sure it's sane
    template_cache templates;
    for (size_t i = 0; i < n; ++i)
      try
	{
	  // NB this calls load_template, so absolute path isn't
	  // necessary
	  templates.push_back
	    (data, show (_m_agramtab->GetPartOfSpeechStr (i)) + ".cs");
	}
      catch (std::runtime_error const &err)
	{
	  templates.push_back (NULL);
	  std::cerr << err.what () << std::endl;
	}

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
	    std::cout << "p.o.s.:   " << pos.number ()
		      << " (" << show (pos.name ()) << ")" << std::endl;

	    lemmatize::forms forms = it.forms ();
	    typedef std::map<std::string, std::vector<std::string> > form_map_t;
	    form_map_t form_map;

	    int gender = -1;
	    for (lemmatize::forms::const_iterator ft = forms.begin ();
		 ft != forms.end (); ++ft)
	      {
		std::string form = show (*ft, ft.accent ());
		gramcodes ac = ft.ancode ();

		std::vector<grammeme> grammemes = ac.grammemes ();
		std::stringstream ss;

		bool first = true;
		bool is_masculine = false;
		bool is_feminine = false;
		bool is_neuter = false;
		for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
		     gt != grammemes.end (); ++gt)
		  {
		    // For nouns, we want the information about gender
		    // extracted and stored separately.
		    if (pos.number () == pos_noun)
		      switch (gt->value ())
			{
			case gm_masculine:
			  is_masculine = true;
			  continue;
			case gm_feminine:
			  is_feminine = true;
			  continue;
			case gm_neuter:
			  is_neuter = true;
			  continue;
			case gm_masc_femin:
			  is_masculine = true;
			  is_feminine = true;
			  continue;
			}

		    ss << (first ? "" : ",") << gt->c_str ();
		    first = false;
		  }

		// no categories?
		if (grammemes.begin () == grammemes.end ())
		  ss << "_";
		std::string category = show (ss.str ());

		if (pos.number () == pos_noun)
		  {
		    if (is_masculine && is_feminine && !is_neuter)
		      gender = gm_masc_femin;
		    else if (is_masculine && !is_feminine && !is_neuter)
		      gender = gm_masculine;
		    else if (!is_masculine && is_feminine && !is_neuter)
		      gender = gm_feminine;
		    else if (!is_masculine && !is_feminine && is_neuter)
		      gender = gm_neuter;
		    else
		      std::cout << "warning: unknown noun gender (masc="
				<< is_masculine << ", fem=" << is_feminine
				<< ", n=" << is_neuter << ")." << std::endl;
		  }

		form_map[category].push_back (form);
	      }

	    for (form_map_t::const_iterator it = form_map.begin ();
		 it != form_map.end (); ++it)
	      {
		std::string name = std::string ("Form.") + it->first;
		HDF *node;
		handle_neoerr (hdf_get_node (data, name.c_str (), &node));
		for (size_t i = 0; i < it->second.size (); ++i)
		  handle_neoerr (hdf_set_valuef (node, "%zd=%s", i,
						 it->second[i].c_str ()));
	      }

	    if (gender != -1)
	      {
		grammeme (_m_agramtab, gender);
		std::string gender_str = show (grammeme (_m_agramtab,
							 gender).c_str ());
		hdf_set_value (data, "gender", gender_str.c_str ());
	      }

	    handle_neoerr (hdf_dump (data, ">"));

	    struct _
	    {
	      static NEOERR *render_cb (void *data, char *str)
	      {
		std::cout << str;
		return NULL;
	      }
	    };

	    CSPARSE *tmpl = templates.at (pos.number ());
	    if (tmpl != NULL)
	      handle_neoerr (cs_render (tmpl, this, &_::render_cb));
	    else
	      std::cout << "No template for " << show (pos.name ())
			<< "." << std::endl;
	    handle_neoerr (hdf_remove_tree (data, "Form"));

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
