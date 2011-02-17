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

#include "number.hh"
#include "format.hh"
#include "forms.hh"
#include "rus_gramtab.hh"

#include <boost/format.hpp>
#include <iostream>

number_handler::number_handler (id_allocator &parent)
  : pos_handler (parent, "number")
{
}

void
number_handler::fill_hdf (CAgramtab *agramtab,
			  lemmatize::const_iterator const &it,
			  hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      std::vector<grammeme> gs = ft.ancode ().grammemes ();

      gram_code_t gender = gm__invalid;
      gram_code_t gcase = gm__invalid;

      for (std::vector<grammeme>::const_iterator gt = gs.begin ();
	   gt != gs.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  if (!extract_rus_gender (gender, code)
	      && !extract_rus_case (gcase, code))
	    std::cerr << (boost::format ("Unhandled grammeme %s\n")
			  % format_rus (code));
	}

      std::string key;
      if (gender == gm__invalid)
	// assume plural
	key = str (boost::format ("%s.%s")
		   % format_rus (gm_plural)
		   % format_rus (gcase));
      else
	key = str (boost::format ("%s.%s")
		   % format_rus (gender)
		   % format_rus (gcase));

      data[key].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
