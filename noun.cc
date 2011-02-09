#include "noun.hh"
#include "forms.hh"
#include "rus_gramtab.hh"
#include "format.hh"

#include <boost/format.hpp>

namespace
{
  gram_code_t
  get_gender (lemmatize::forms::const_iterator form)
  {
    gramcodes ac = form.ancode ();
    std::vector<grammeme> grammemes = ac.grammemes ();

    bool is_masculine = false;
    bool is_feminine = false;
    bool is_neuter = false;
    for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
	 gt != grammemes.end (); ++gt)
      switch (gt->value ())
	{
	case gm_masculine:
	  is_masculine = true;
	  continue;
	case gm_feminine:
	  is_feminine = true;
	  continue;
	case gm_neuter:
	  is_neuter = true;
	  continue;
	case gm_masc_femin:
	  is_masculine = true;
	  is_feminine = true;
	  continue;
	}

    if (is_masculine && is_feminine && !is_neuter)
      return gm_masc_femin;
    if (is_masculine && !is_feminine && !is_neuter)
      return gm_masculine;
    if (!is_masculine && is_feminine && !is_neuter)
      return gm_feminine;
    if (!is_masculine && !is_feminine && is_neuter)
      return gm_neuter;

    return gm__invalid;
  }
}

noun_handler::noun_handler ()
  : pos_handler ("noun")
{}

void
noun_handler::fill_hdf (CAgramtab *agramtab,
			lemmatize::const_iterator const &it,
			hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();
  gram_code_t gender = get_gender (forms.begin ());
  if (gender == gm__invalid)
    throw std::runtime_error ("Can't determine gender.");

  char const *word = format_rus_grammeme_value (gender);
  data["gender"].push_back (std::make_pair (word, -1));

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
			       % format_rus_grammeme_value (gnumber)
			       % format_rus_grammeme_value (gcase));

      data[field].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
