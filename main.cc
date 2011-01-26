#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iconv.h>
#include <iostream>
#include <langinfo.h>
#include <stdexcept>
#include <sstream>

#include <AgramtabLib/RusGramTab.h>
#include "lemmatize.hh"
#include "gramcodes.hh"
#include "forms.hh"

#include <ctemplate/template.h>

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

  int run ()
  {
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
	    std::cout << "p.o.s.:   " << it.part_of_speech ()
		      << " (" << show (it.part_of_speech_str ()) << ")" << std::endl;

	    lemmatize::forms forms = it.forms ();
	    std::cout << "all forms:" << it->GetHomonymWeight () << std::endl;

	    ctemplate::TemplateDictionary *
	      dict = new ctemplate::TemplateDictionary("section example");

	    for (lemmatize::forms::const_iterator ft = forms.begin ();
		 ft != forms.end (); ++ft)
	      {
		std::string form = show (*ft, ft.accent ());
		std::cout << "\t" << form;
		gramcodes ac = ft.ancode ();

		std::vector<grammeme> grammemes = ac.grammemes ();
		std::stringstream ss;
		for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
		     gt != grammemes.end (); ++gt)
		  ss << (gt == grammemes.begin () ? "" : ",") << gt->c_str ();
		std::string raw_category = ss.str ();
		std::string category = show (raw_category);

		dict->SetValue(category, form);
		std::cout << "\t" << category << "(" << grammemes.size () << ")";
		std::cout << std::endl;
	      }

    	    string output;
	    if (!ctemplate::ExpandTemplate ("/home/petr/proj/rus/template.tpl",
					    ctemplate::DO_NOT_STRIP,
					    dict, &output))
	      abort ();
	    std::cout << output << std::endl;
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
