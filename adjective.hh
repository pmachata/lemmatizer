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

  static std::string format_form_key (gram_code_t degree, gram_code_t gcase,
				      gram_code_t number, gram_code_t gender,
				      bool animate, bool inanimate,
				      bool secondary);
};

class short_adjective_handler
  : public delegating_pos_handler<pos_adjective>
{
};

#endif//ADJECTIVE_HH
