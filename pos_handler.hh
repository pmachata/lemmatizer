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

#define FAIL(T) (assert (false), (T)NULL)

#endif//POS_HANDLER_HH
