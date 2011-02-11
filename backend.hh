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

#ifndef BACKEND_HH
#define BACKEND_HH

#include <iosfwd>

class backend
{
public:
  virtual ~backend () {}
  virtual bool get_word (std::string &word) = 0;
  virtual bool render (char const *str) = 0;
  virtual void before_render () {}
  virtual void after_render () {}
};

class console_backend
  : public backend
{
  std::istream &_m_is;
  std::ostream &_m_os;

public:
  console_backend (std::istream &is, std::ostream &os);
  virtual bool get_word (std::string &word);
  virtual bool render (char const *str);
void
before_render ()
{
  render ("Content-type: text/html\n\n");
}

};

#endif//BACKEND_HH
