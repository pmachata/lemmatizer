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

#ifndef GRAMCODES_HH
#define GRAMCODES_HH

#include <string>
#include <AgramtabLib/agramtab_.h>

#include "part_of_speech.ii"

class gramcodes;

class grammeme
{
  CAgramtab *_m_agramtab;
  size_t _m_idx;

public:
  grammeme (CAgramtab *agramtab, size_t idx);
  grammeme (grammeme const &copy);
  grammeme &operator= (grammeme &copy);

  char const *c_str () const;
  size_t value () const;

  // You would call this with T being an enum implementing values for
  // language of your choice.
  template<class T> T value_as () const
  {
    return static_cast<T> (value ());
  }
};

class gramcodes
{
  friend class grammeme;
  CAgramtab *_m_agramtab;
  std::string _m_gramcodes;

public:
  gramcodes (CAgramtab *agramtab, std::string const &gramcodes);
  ~gramcodes ();
  part_of_speech get_part_of_speech () const;
  std::vector<grammeme> grammemes () const;

  class const_iterator;
  const_iterator begin () const;
  const_iterator end () const;
};

class gramcodes::const_iterator
{
  friend class gramcodes;
  gramcodes const &_m_gc;
  size_t _m_idx;

  const_iterator (gramcodes const &gc, size_t idx);

public:
  const_iterator (const_iterator const &copy);
  const_iterator &operator= (const_iterator &copy);

  bool operator== (const_iterator const &other) const;
  bool operator!= (const_iterator const &other) const;
  bool operator< (const_iterator const &other) const;

  const_iterator& operator++ ();
  const_iterator operator++ (int);

  const_iterator& operator-- ();
  const_iterator operator-- (int);

  char const *operator* () const;
  std::vector<grammeme> grammemes () const;
};

#endif//GRAMCODES_HH
