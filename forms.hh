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
