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

#include <boost/format.hpp>

#include "default.hh"
#include "forms.hh"
#include "format.hh"
#include "adjective.hh"

default_handler::default_handler (id_allocator &parent)
  : pos_handler (parent, "default")
{
}

void
default_handler::fill_hdf (CAgramtab *agramtab,
			   lemmatize::const_iterator const &it,
			   hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      size_t next_id = data["forms"].size ();
      std::string sub_key = str (boost::format ("forms.%d.attributes")
				 % next_id);
      std::vector<grammeme> gs = ft.ancode ().grammemes ();
      for (std::vector<grammeme>::const_iterator gt = gs.begin ();
	   gt != gs.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  data[sub_key].push_back (std::make_pair (format_rus (code), -1));
	}
      data["forms"].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
