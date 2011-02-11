#include "noun.hh"
#include "forms.hh"
#include "rus_gramtab.hh"
#include "format.hh"
#include "rus_gender.hh"

#include <boost/format.hpp>
#include <bitset>

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
      std::vector<grammeme> grammemes = ac.grammemes ();
      for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
	   gt != grammemes.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  extract_rus_number (gnumber, code);
	  extract_rus_case (gcase, code);
	}

      if (gnumber == -1)
	throw std::runtime_error ("Can't determine number.");
      if (gcase == -1)
	throw std::runtime_error ("Can't determine case.");

      std::string field = str (boost::format ("%s.%s")
			       % format_rus (gnumber)
			       % format_rus (gcase));

      data[field].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
