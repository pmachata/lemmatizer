// Copyright (C) 2011 Petr Machata
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

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

  // XXX get rid of this.  It's useless now that we can share handlers
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
