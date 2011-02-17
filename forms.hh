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

#ifndef FORMS_HH
#define FORMS_HH

#include <AgramtabLib/agramtab_.h>
#include <LemmatizerLib/Lemmatizers.h>

#include "lemmatize.hh"
#include "gramcodes.hh"

class lemmatize::forms
{
  friend class lemmatize::const_iterator;
  CAgramtab *_m_agramtab;
  CFormInfo const &_m_paradigm;

  forms (CAgramtab *agramtab, CFormInfo const &paradigm);

public:
  forms (forms const &copy);

  std::string operator[] (size_t idx);

  int homonym_weight () const;
  int word_weight () const;
  size_t size () const;

  class const_iterator;
  const_iterator begin () const;
  const_iterator end () const;
};

class lemmatize::forms::const_iterator
{
  friend class lemmatize::forms;
  lemmatize::forms const &_m_forms;
  size_t _m_idx;

  const_iterator (lemmatize::forms const &forms, size_t idx);

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

  int accent () const;
  std::string operator* () const;
  gramcodes ancode () const;
  int homonym_weight () const;
};

#endif//FORMS_HH
