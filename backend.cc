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
