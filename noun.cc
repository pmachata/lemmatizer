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

#include "noun.hh"
#include "forms.hh"
#include "rus_gramtab.hh"
#include "format.hh"
#include "rus_gender.hh"

#include <boost/format.hpp>
#include <bitset>
#include <iostream>

noun_handler::noun_handler ()
  : pos_handler ("noun")
{}

void
noun_handler::fill_hdf (CAgramtab *agramtab,
			lemmatize::const_iterator const &it,
			hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();

  gender_set_t genders = get_gender (forms.begin ().ancode ());
  for (size_t i = 0; i < genders.size (); ++i)
    if (genders[i])
      {
	char const *name = format_rus (gender_to_gramcode[i]);
	data["gender"].push_back (std::make_pair (name, -1));
      }

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      gramcodes ac = ft.ancode ();

      gram_code_t gnumber = gm__invalid;
      gram_code_t gcase = gm__invalid;
      bool indeclinable = false;
      gram_code_t gender;
      std::vector<grammeme> grammemes = ac.grammemes ();
      for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
	   gt != grammemes.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  if (!extract_rus_number (gnumber, code)
	      && !extract_rus_case (gcase, code))
	    {
	      if (code == gm_indeclinable)
		indeclinable = true;
	      else if (!extract_rus_gender (gender, code))
		std::cerr << (boost::format ("Unhandled grammeme %s\n")
			      % format_rus (code));
	    }
	}

      if (gnumber == -1)
	throw std::runtime_error ("Can't determine number.");
      if (gcase == -1)
	throw std::runtime_error ("Can't determine case.");

      // For indeclinable nouns, Seman for some reason reports only
      // vocative case.  Change it to nominative.
      if (indeclinable && gcase == gm_vocative)
	gcase = gm_nominative;

      std::string field = str (boost::format ("%s.%s")
			       % format_rus (gnumber)
			       % format_rus (gcase));

      data[field].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
