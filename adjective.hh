#ifndef ADJECTIVE_HH
#define ADJECTIVE_HH

#include "pos_handler.hh"
#include "rus_gramtab.hh"

class adjective_handler
  : public pos_handler
{
public:
  adjective_handler ();
  virtual void fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const;
};

class short_adjective_handler
  : public delegating_pos_handler<pos_adjective>
{
};

#endif//ADJECTIVE_HH
