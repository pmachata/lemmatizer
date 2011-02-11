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

#include "lemmatize.hh"
#include "forms.hh"
#include "part_of_speech.hh"

lemmatize::lemmatize (std::string word,
		      CLemmatizer *lemmatizer, CAgramtab *agramtab)
  : _m_agramtab (agramtab)
{
  lemmatizer->CreateParadigmCollection (false, word, false,
					true, _m_paradigms);
}

lemmatize::const_iterator::const_iterator
(std::vector<CFormInfo>::const_iterator it, CAgramtab *agramtab)
  : _m_it (it)
  , _m_agramtab (agramtab)
  , _m_ancode (NULL)
{
}

lemmatize::const_iterator
lemmatize::begin () const
{
  return const_iterator (_m_paradigms.begin (), _m_agramtab);
}

lemmatize::const_iterator
lemmatize::end () const
{
  return const_iterator (_m_paradigms.end (), NULL);
}


lemmatize::const_iterator::const_iterator (const_iterator const &copy)
  : _m_it (copy._m_it)
  , _m_agramtab (copy._m_agramtab)
  , _m_ancode (NULL)
{
}

lemmatize::const_iterator::~const_iterator ()
{
  uncache ();
}

void
lemmatize::const_iterator::uncache ()
{
  delete _m_ancode;
  _m_ancode = NULL;
}

lemmatize::const_iterator &
lemmatize::const_iterator::operator= (const_iterator const &copy)
{
  _m_it = copy._m_it;
  uncache ();
  return *this;
}

bool
lemmatize::const_iterator::operator== (const_iterator const &other) const
{
  return _m_it == other._m_it;
}

bool
lemmatize::const_iterator::operator!= (const_iterator const &other) const
{
  return _m_it != other._m_it;
}

bool
lemmatize::const_iterator::operator< (const_iterator const &other) const
{
  return _m_it < other._m_it;
}

lemmatize::const_iterator &
lemmatize::const_iterator::operator++ () //prefix
{
  uncache ();
  ++_m_it;
  return *this;
}

lemmatize::const_iterator
lemmatize::const_iterator::operator++ (int) //suffix
{
  const_iterator copy = *this;
  uncache ();
  ++*this;
  return copy;
}

lemmatize::const_iterator &
lemmatize::const_iterator::operator-- () //prefix
{
  uncache ();
  --_m_it;
  return *this;
}

lemmatize::const_iterator
lemmatize::const_iterator::operator-- (int) //suffix
{
  const_iterator copy = *this;
  uncache ();
  --*this;
  return copy;
}

CFormInfo const &
lemmatize::const_iterator::operator* () const
{
  return *_m_it;
}

CFormInfo const *
lemmatize::const_iterator::operator-> () const
{
  return &*_m_it;
}

CFormInfo const &
lemmatize::const_iterator::paradigm () const
{
  return **this;
}

long
lemmatize::const_iterator::paradigm_id () const
{
  return paradigm ().GetParadigmId ();
}

bool
lemmatize::const_iterator::found () const
{
  return paradigm ().m_bFound;
}

gramcodes
lemmatize::const_iterator::ancode () const
{
  return gramcodes (_m_agramtab,
		    paradigm ().GetSrcAncode ());
}

gramcodes
lemmatize::const_iterator::common_ancode ()
{
  return gramcodes (_m_agramtab,
		    paradigm ().GetCommonAncode ());
}

part_of_speech
lemmatize::const_iterator::get_part_of_speech () const
{
  return ancode ().get_part_of_speech ();
}

int
lemmatize::const_iterator::accent () const
{
  return (signed char)paradigm ().GetSrcAccentedVowel ();
}

lemmatize::forms
lemmatize::const_iterator::forms () const
{
  return lemmatize::forms (_m_agramtab, paradigm ());
}
