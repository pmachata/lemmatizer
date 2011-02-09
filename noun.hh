#ifndef NOUN_HH
#define NOUN_HH

#include "pos_handler.hh"

class noun_handler
  : public pos_handler
{
public:
  noun_handler ();

  virtual void fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const;
};

#endif//NOUN_HH
