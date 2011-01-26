#include "gramcodes.hh"
#include <iostream>
#include <iterator>
#include <AgramtabLib/RusGramTab.h>

gramcodes::gramcodes (CAgramtab *agramtab, std::string const &gramcodes)
  : _m_agramtab (agramtab)
  , _m_gramcodes (gramcodes)
{}

gramcodes::~gramcodes ()
{
}

int
gramcodes::part_of_speech () const
{
  return _m_agramtab->GetPartOfSpeech (_m_gramcodes.c_str ());
}

gramcodes::const_iterator
gramcodes::begin () const
{
  return const_iterator (*this, 0);
}

gramcodes::const_iterator
gramcodes::end () const
{
  return const_iterator (*this, _m_gramcodes.length ());
}

std::vector<grammeme>
gramcodes::grammemes () const
{
  std::vector<grammeme> ret;
  for (const_iterator it = begin (); it != end (); ++it)
    {
      std::vector<grammeme> const &sub = it.grammemes ();
      std::copy (sub.begin (), sub.end(), std::back_inserter (ret));
    }
  return ret;
}


grammeme::grammeme (gramcodes const &gc, size_t idx)
  : _m_gc (gc)
  , _m_idx (idx)
{
}

grammeme::grammeme (grammeme const &copy)
  : _m_gc (copy._m_gc)
  , _m_idx (copy._m_idx)
{
}

grammeme &
grammeme::operator= (grammeme &copy)
{
  _m_idx = copy._m_idx;
  return *this;
}

char const *
grammeme::c_str () const
{
  return _m_gc._m_agramtab->GetGrammemStr (_m_idx);
}

size_t
grammeme::value () const
{
  return _m_idx;
}


gramcodes::const_iterator::const_iterator (gramcodes const &gc, size_t idx)
  : _m_gc (gc)
  , _m_idx (idx)
{
}

gramcodes::const_iterator::const_iterator (const_iterator const &copy)
  : _m_gc (copy._m_gc)
  , _m_idx (copy._m_idx)
{}

gramcodes::const_iterator &
gramcodes::const_iterator::operator= (const_iterator &copy)
{
  _m_idx = copy._m_idx;
  return *this;
}

bool
gramcodes::const_iterator::operator== (const_iterator const &other) const
{
  return _m_idx == other._m_idx;
}

bool
gramcodes::const_iterator::operator!= (const_iterator const &other) const
{
  return !(*this == other);
}

bool
gramcodes::const_iterator::operator< (const_iterator const &other) const
{
  return _m_idx < other._m_idx;
}

gramcodes::const_iterator &
gramcodes::const_iterator::operator++ () //prefix
{
  _m_idx += 2;
  return *this;
}

gramcodes::const_iterator
gramcodes::const_iterator::operator++ (int) //suffix
{
  const_iterator copy = *this;
  ++*this;
  return copy;
}

gramcodes::const_iterator &
gramcodes::const_iterator::operator-- () //prefix
{
  _m_idx -= 2;
  return *this;
}

gramcodes::const_iterator
gramcodes::const_iterator::operator-- (int) //suffix
{
  const_iterator copy = *this;
  --*this;
  return copy;
}

char const *
gramcodes::const_iterator::operator* () const
{
  return _m_gc._m_gramcodes.c_str () + _m_idx;
}

#define _QM(X) (((QWORD)1)<<X)

std::vector<grammeme>
gramcodes::const_iterator::grammemes () const
{
  std::vector<grammeme> ret;

  QWORD g = 0;
  _m_gc._m_agramtab->GetGrammems (**this, g);
  for (size_t i = 0; i < _m_gc._m_agramtab->GetGrammemsCount (); ++i)
    if (_QM(i) & g)
      ret.push_back (grammeme (_m_gc, i));

  return ret;
}
