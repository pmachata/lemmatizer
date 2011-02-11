#ifndef VERB_HH
#define VERB_HH

#include "pos_handler.hh"

class verb_handler
  : public pos_handler
{
public:
  verb_handler ();
  virtual void fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const;
};

#endif//VERB_HH
