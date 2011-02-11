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

#ifndef LEMMATIZE_HH
#define LEMMATIZE_HH

#include <vector>
#include <string>
#include <AgramtabLib/agramtab_.h>
#include <LemmatizerLib/Lemmatizers.h>

#include "gramcodes.ii"
#include "part_of_speech.ii"

class lemmatize
{
  CAgramtab *_m_agramtab;
  std::vector<CFormInfo> _m_paradigms;

public:
  lemmatize (std::string word,
	     CLemmatizer *lemmatizer, CAgramtab *agramtab);

  class const_iterator;
  const_iterator begin () const;
  const_iterator end () const;

  class forms;
};

class lemmatize::const_iterator
{
  friend class lemmatize;
  std::vector<CFormInfo>::const_iterator _m_it;

  CAgramtab *_m_agramtab;
  mutable gramcodes *_m_ancode;

  const_iterator (std::vector<CFormInfo>::const_iterator it,
		  CAgramtab *agramtab);

  void uncache ();

public:
  const_iterator (const_iterator const &copy);
  ~const_iterator ();
  const_iterator &operator= (const_iterator const &copy);

  bool operator== (const_iterator const &other) const;
  bool operator!= (const_iterator const &other) const;
  bool operator< (const_iterator const &other) const;

  const_iterator& operator++ ();
  const_iterator operator++ (int);
  const_iterator& operator-- ();
  const_iterator operator-- (int);

  CFormInfo const &operator* () const;
  CFormInfo const *operator-> () const;
  CFormInfo const &paradigm () const;

  long paradigm_id () const;
  bool found () const;

  gramcodes ancode () const;
  gramcodes common_ancode ();

  part_of_speech get_part_of_speech () const;

  int accent () const;

  lemmatize::forms forms () const;
};

#endif//LEMMATIZE_HH
