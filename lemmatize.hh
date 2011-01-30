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
