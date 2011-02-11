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

#ifndef PART_OF_SPEECH_HH
#define PART_OF_SPEECH_HH

#include <string>
#include <AgramtabLib/agramtab_.h>

class part_of_speech
{
  CAgramtab *_m_agramtab;
  int _m_part_of_speech;

public:
  part_of_speech (CAgramtab *agramtab, int pos)
    : _m_agramtab (agramtab)
    , _m_part_of_speech (pos)
  {
  }

  part_of_speech (part_of_speech const &copy)
    : _m_agramtab (copy._m_agramtab)
    , _m_part_of_speech (copy._m_part_of_speech)
  {
  }

  part_of_speech &
  operator= (part_of_speech const &copy)
  {
    _m_agramtab = copy._m_agramtab;
    _m_part_of_speech = copy._m_part_of_speech;
    return *this;
  }

  int
  number () const
  {
    return _m_part_of_speech;
  }

  template <class T> T number_as () const
  {
    return static_cast<T> (number ());
  }

  bool
  operator< (part_of_speech const &other) const
  {
    assert (_m_agramtab == other._m_agramtab);
    return _m_part_of_speech < other._m_part_of_speech;
  }

  std::string name () const;
};

#endif//PART_OF_SPEECH_HH
