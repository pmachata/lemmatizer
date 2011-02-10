//
//   Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Free Software
//   Foundation, Inc
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// This snippet was taken from the Gnash project.  It's covered by the
// license above.

#include <string>

void
url_decode(std::string& input)
{
  int hexcode;

  for (unsigned int i=0; i<input.length(); i++)
    {
      if (input[i] == '%' && (input.length() > i + 2) &&
	  isxdigit(input[i+1]) && isxdigit(input[i+2]))
	{
	  input[i+1] = toupper(input[i+1]);
	  input[i+2] = toupper(input[i+2]);
	  if (isdigit(input[i+1]))
	    hexcode = (input[i+1] - '0') * 16;
	  else
	    hexcode = (toupper (input[i+1]) - 'A' + 10) * 16;

	  if (isdigit(input[i+2]))
	    hexcode += (input[i+2] - '0');
	  else
	    hexcode += (toupper (input[i+2]) - 'A' + 10);

	  input[i] = (char)hexcode;
	  input.erase(i+1, 2);
	}
      else if ( input[i] == '+' )
	{
	  input[i] = ' ';
	}
    }
}
