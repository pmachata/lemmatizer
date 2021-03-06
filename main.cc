// Copyright (C) 2011 Petr Machata
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <cstring>
#include <iconv.h>
#include <iostream>
#include <cerrno>

#include <AgramtabLib/RusGramTab.h>

#include <cs/cs.h>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include "backend.hh"
#include "fcgi_backend.hh"
#include "format.hh"
#include "forms.hh"
#include "hdf.hh"
#include "lemmatize.hh"
#include "main.hh"
#include "part_of_speech.hh"
#include "pos_handler.hh"
#include "rus_gramtab.hh"
#include "template_cache.hh"

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

// convert windows1251-encoded character to lowercase
char
windows1251_lowercase (char c0)
{
  unsigned char c = (unsigned char)c0;
  if (__builtin_expect ((c >= 0xc0 && c < 0xe0), 1))
    return c + 0x20;

  switch (c)
    {
    case 0xa1: // short u
    case 0xb2: // pre-reform russian i
    case 0xbd: // s
      return c + 1;

    case 0x81: // gje
      return c + 2;

    case 0x80: // dje
    case 0x8a: // lje
    case 0x8c: // nje
    case 0x8d: // kje
    case 0x8e: // tshe
    case 0x8f: // dzhe
    case 0xa8: // yo
    case 0xaa: // ye
    case 0xaf: // yi
      return c + 0x10;

    case 0xa3: // je
      return 0xbc;

    case 0xa5: // ge with upturn
      return 0xb4;
    }

  return std::tolower (c);
}

// convert windows1251-encoded string to lowercase
std::string
windows1251_lowercase (std::string w)
{
  std::transform (w.begin (), w.end (), w.begin (),
		  (char (&)(char))windows1251_lowercase);
  return w;
}

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
  template_cache _m_templates;
  pos_handler_map _m_handlers;
  default_hdf _m_hdf;
  size_t _m_page_id;
  HDF *_m_app_hdf; // Sub-node of _m_hdf holding app data.

  template <class T>
  T check (T retval, std::string const &reason)
  {
    if (retval == (T)-1)
      fail (reason);
    return retval;
  }

  CSPARSE *
  get_template (size_t id, char const *fn)
  {
    CSPARSE *tmpl = _m_templates.get (id);
    if (tmpl == NULL)
      {
	std::string file_name = fn;
	file_name += ".cs";
	try
	  {
	    tmpl = _m_templates.add (_m_hdf, id,
				     file_name.c_str ());
	    if (tmpl == NULL)
	      throw std::runtime_error ("Unknown reason.");
	  }
	catch (std::runtime_error const &err)
	  {
	    std::cerr << (boost::format
			  ("Couldn't load the template `%s': %s.\n")
			  % file_name % err.what ());
	    return NULL;
	  }
	assert (tmpl != NULL);
      }
    return tmpl;
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
    , _m_handlers (_m_templates)
    , _m_page_id (_m_templates.get_id ())
    , _m_app_hdf (_m_hdf.node ("lemmatizer"))
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
  std::string show (std::string const &w1, int accent = -1)
  {
    std::string w = windows1251_lowercase (w1);

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
    std::string contents;
    CSPARSE *page = get_template (_m_page_id, "page");

    struct cb
    {
      static NEOERR *
      backend_render (void *data, char *str)
      {
	backend *b = (backend *)data;
	bool result = b->render (str);

	if (result)
	  return NULL;
	else
	  return nerr_raise (NERR_PARSE, "Backend render failed.");
      }

      static NEOERR *
      append_contents (void *data, char *str)
      {
	std::string *contents = (std::string *)data;
	*contents += str;
	return NULL;
      }
    };

    lemmatize lem (line, _m_lemmatizer, _m_agramtab);
    for (lemmatize::const_iterator it = lem.begin ();
	 it != lem.end (); ++it)
      {
	part_of_speech pos = it.get_part_of_speech ();
	std::cerr << (boost::format ("%s: %s\n")
		      % show (line) % show (pos.name ()));

	pos_handler const *handler
	  = _m_handlers.get_handler (pos.number ());
	if (handler == NULL)
	  {
	    std::cerr << "No handler for part of speech \""
		      << show (pos.name ()) << "\" ("
		      << pos.number () << ")." << std::endl;
	    continue;
	  }

	CSPARSE *tmpl = get_template (handler->id (),
				      handler->template_name ());
	if (tmpl == NULL)
	  continue;

	hdf_data_map data;
	handler->fill_hdf (_m_agramtab, it, data);

	handle_neoerr (hdf_set_valuef (_m_hdf, "Form.found=%d",
				       (int)it.found ()));
	// We want English string here, not the silly short Russian
	// ones that they provide in Seman.
	char const *pos_string = format_rus (pos.number_as<pos_code_t> ());
	handle_neoerr (hdf_set_value (_m_hdf, "Form.pos", pos_string));
	handle_neoerr (hdf_set_value (_m_hdf, "Form.source",
				      show (it->GetSrcNorm ()).c_str ()));

	for (hdf_data_map::const_iterator it = data.begin ();
	     it != data.end (); ++it)
	  {
	    std::string name = std::string ("Form.") + show (it->first);
	    HDF *node = _m_hdf.node (name.c_str ());
	    for (size_t i = 0; i < it->second.size (); ++i)
	      {
		std::string const &word = it->second[i].first;
		int accent = it->second[i].second;
		std::string final = show (word, accent);
		handle_neoerr (hdf_set_valuef (node, "%zd=%s",
					       i, final.c_str ()));
	      }
	  }

	if (hdf_get_int_value (_m_app_hdf, "dump", 0))
	  handle_neoerr (hdf_dump (_m_hdf.node ("Form"), "Form"));

	handle_neoerr (cs_render (tmpl, &contents, &cb::append_contents));
	handle_neoerr (hdf_remove_tree (_m_hdf, "Form"));
      }

    handle_neoerr (hdf_set_value (_m_hdf, "page.contents", contents.c_str ()));
    handle_neoerr (hdf_set_value (_m_hdf, "page.word", show (line).c_str ()));
    handle_neoerr (cs_render (page, back, &cb::backend_render));
    handle_neoerr (hdf_remove_tree (_m_hdf, "page"));
  }

  int
  loop (backend *back)
  {
    std::string line;
    while (true)
      try
	{
	  if (!back->get_word (line))
	    break;
	  back->before_render ();

	  line = convert (line, _m_iconv_to);
	  Trim (line);
	  if (line.empty ())
	    continue;

	  // XXX here we would perhaps like to convert the string to
	  // lower case.

	  process (line, back);
	  back->after_render ();
	}
      catch (CExpc const &exc)
	{
	  std::cerr << boost::format ("seman exception: %s\n") % exc.m_strCause;
	}
      catch (std::runtime_error const &err)
	{
	  std::cerr << boost::format ("runtime error: %s\n") % err.what ();
	}
      catch (...)
	{
	  std::cerr << "unknown exception\n";
	}
    return 0;
  }
};

namespace po = boost::program_options;

int main(int argc, char **argv)
{
  setlocale (LC_ALL, "");
  //std::cerr << "LC_ALL=" << setlocale (LC_ALL, NULL) << std::endl;

  po::options_description desc ("LemmatizerWeb");
  desc.add_options()
    ("help", "show this help message")
    ("io", po::value<std::string>(), "set IO backend {fcgi|console}")
    ;

  po::variables_map vm;
  po::store (po::parse_command_line (argc, argv, desc), vm);
  po::notify (vm);

  if (vm.count ("help"))
    {
      std::cout << desc << "\n";
      return 1;
    }

  backend *back = NULL;
  std::string backend = vm.count ("io") ? vm["io"].as<std::string> () : "fcgi";
  if (backend == "console")
    back = new console_backend (std::cin, std::cout);
  else if (backend == "fcgi")
    back = new fcgi_backend ();
  else
    {
      std::cerr << boost::format ("Invalid IO backend `%s'.\n") % backend;
      return 1;
    }

  int ret = lemmatizer_app ().loop (back);
  delete back;

  return ret;
}
