#include "fcgi_backend.hh"
#include <fcgi_stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>

extern "C" char **environ;

bool
fcgi_backend::get_word (std::string &word)
{
  if (FCGI_Accept () < 0)
    return false;

  static char QS[] = "QUERY_STRING=";

  for (char **it = environ; *it != NULL; ++it)
    if (strncmp (*it, QS, sizeof (QS) - 1) == 0)
      {
	word = *it + sizeof (QS) - 1;
	word = "\xd0\xb7\xd0\xb0\xd0\xb2\xd1\x82\xd1\x80\xd0\xb0";
	return true;
      }

  return false;
}

void
fcgi_backend::before_render ()
{
  render ("Content-type: text/html\r\n\r\n");
}

bool
fcgi_backend::render (char const *str)
{
  return FCGI_printf ("%s", str) >= 0;
}