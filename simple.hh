#ifndef SIMPLE_HH
#define SIMPLE_HH

#include "pos_handler.hh"

class simple_handler
  : public pos_handler
{
public:
  simple_handler ();

  virtual void fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const;
};

#endif//SIMPLE_HH
