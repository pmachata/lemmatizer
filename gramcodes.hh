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
