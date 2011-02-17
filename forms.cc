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

#include "forms.hh"

lemmatize::forms::forms (CAgramtab *agramtab, CFormInfo const &paradigm)
  : _m_agramtab (agramtab)
  , _m_paradigm (paradigm)
{
}

lemmatize::forms::forms (forms const &copy)
  : _m_agramtab (copy._m_agramtab)
  , _m_paradigm (copy._m_paradigm)
{
}

std::string
lemmatize::forms::operator[] (size_t idx)
{
  return _m_paradigm.GetWordForm (idx);
}


lemmatize::forms::const_iterator
lemmatize::forms::begin () const
{
  return const_iterator (*this, 0);
}

lemmatize::forms::const_iterator
lemmatize::forms::end () const
{
  return const_iterator (*this, size ());
}

int
lemmatize::forms::homonym_weight () const
{
  return _m_paradigm.GetHomonymWeight ();
}

int
lemmatize::forms::word_weight () const
{
  return _m_paradigm.GetWordWeight ();
}

size_t
lemmatize::forms::size () const
{
  return _m_paradigm.GetCount ();
}


lemmatize::forms::const_iterator::const_iterator \
(lemmatize::forms const &forms, size_t idx)
  : _m_forms (forms)
  , _m_idx (idx)
{}

lemmatize::forms::const_iterator::const_iterator (const_iterator const &copy)
  : _m_forms (copy._m_forms)
  , _m_idx (copy._m_idx)
{}

lemmatize::forms::const_iterator &
lemmatize::forms::const_iterator::operator= (const_iterator &copy)
{
  _m_idx = copy._m_idx;
  return *this;
}

bool
lemmatize::forms::const_iterator::operator== (const_iterator const &other) const
{
  return _m_idx == other._m_idx;
}

bool
lemmatize::forms::const_iterator::operator!= (const_iterator const &other) const
{
  return !(*this == other);
}

bool
lemmatize::forms::const_iterator::operator< (const_iterator const &other) const
{
  return _m_idx < other._m_idx;
}

lemmatize::forms::const_iterator &
lemmatize::forms::const_iterator::operator++ () //prefix
{
  ++_m_idx;
  return *this;
}

lemmatize::forms::const_iterator
lemmatize::forms::const_iterator::operator++ (int) //suffix
{
  const_iterator copy = *this;
  ++*this;
  return copy;
}

lemmatize::forms::const_iterator &
lemmatize::forms::const_iterator::operator-- () //prefix
{
  --_m_idx;
  return *this;
}

lemmatize::forms::const_iterator
lemmatize::forms::const_iterator::operator-- (int) //suffix
{
  const_iterator copy = *this;
  --*this;
  return copy;
}

int
lemmatize::forms::const_iterator::accent () const
{
  return (signed char)_m_forms._m_paradigm.GetAccentedVowel (_m_idx);
}

std::string
lemmatize::forms::const_iterator::operator* () const
{
  return _m_forms._m_paradigm.GetWordForm (_m_idx);
}

gramcodes
lemmatize::forms::const_iterator::ancode () const
{
  return gramcodes (_m_forms._m_agramtab,
		    _m_forms._m_paradigm.GetAncode (_m_idx));
}

int
lemmatize::forms::const_iterator::homonym_weight () const
{
  return _m_forms._m_paradigm.GetHomonymWeightWithForm (_m_idx);
}
