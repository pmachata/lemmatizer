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

#include "backend.hh"

#include <iostream>

console_backend::console_backend (std::istream &is, std::ostream &os)
  : _m_is (is)
  , _m_os (os)
{}

bool
console_backend::get_word (std::string &word)
{
  std::getline (_m_is, word);
  return !std::cin.eof ();
}

bool
console_backend::render (char const *str)
{
  _m_os << str;
  return true;
}
