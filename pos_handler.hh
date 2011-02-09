#ifndef POS_HANDLER_HH
#define POS_HANDLER_HH

#include <map>
#include <string>
#include <vector>

#include <AgramtabLib/agramtab_.h>

#include "lemmatize.hh"

// For what should be entered to HDF.  That's:
//  key -> [(word, accent position)]
typedef std::map<std::string,
		 std::vector<std::pair<std::string, int> > > hdf_data_map;

class pos_handler
{
  char const *_m_name;
  int _m_delegate;

public:
  explicit pos_handler (char const *name);
  explicit pos_handler (int delegate);

  char const *template_name () const;
  int delegate () const;

  virtual void fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const;
};

template<int Delegate>
struct delegating_pos_handler
  : public pos_handler
{
  delegating_pos_handler()
    : pos_handler (Delegate)
  {}
};

class pos_handler_map
  : private std::map<int, pos_handler const *>
{
public:
  pos_handler_map ();
  ~pos_handler_map ();
  virtual pos_handler const *get_handler (int pos);
};

#define FAIL(T) (assert (false), (T)NULL)

#endif//POS_HANDLER_HH
