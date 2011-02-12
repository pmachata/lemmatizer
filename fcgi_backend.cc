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

#include "fcgi_backend.hh"
#include "url_decode.hh"

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

	// Arbitrary cutoff.  While it seems to not be a problem for
	// Seman, nobody really needs to lemmatize words this long.
	// Note that since urlencode is in effect, this really is just
	// about 30 characters.  This should still be enough for
	// sensible words.
	if (word.size () > 100)
	  word = "";

	url_decode (word);
	return true;
      }

  word = "";
  return true;
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
