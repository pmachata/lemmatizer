#include "noun.hh"
#include "forms.hh"
#include "rus_gramtab.hh"

#include <boost/format.hpp>

namespace
{
  int
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

    return -1;
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
  int gender = get_gender (forms.begin ());
  if (gender == -1)
    throw std::runtime_error ("Can't determine gender.");

  char const *word = gender == gm_masculine ? "masculine"
    : gender == gm_feminine ? "feminine"
    : gender == gm_neuter ? "neuter"
    : gender == gm_masc_femin ? "masculine/feminine"
    : FAIL (char const *);
  data["gender"].push_back (std::make_pair (word, -1));

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      gramcodes ac = ft.ancode ();

      int gnumber = -1;
      int gcase = -1;
      std::vector<grammeme> grammemes = ac.grammemes ();
      for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
	   gt != grammemes.end (); ++gt)
	switch (gt->value ())
	  {
	  case gm_plural:
	  case gm_singular:
	    gnumber = gt->value ();
	    continue;

	  case gm_nominative:
	  case gm_genitive:
	  case gm_dative:
	  case gm_accusative:
	  case gm_instrumental:
	  case gm_prepositional:
	  case gm_vocative:
	    gcase = gt->value ();
	    continue;
	  }

      if (gnumber == -1)
	throw std::runtime_error ("Can't determine number.");
      if (gcase == -1)
	throw std::runtime_error ("Can't determine case.");

      std::string field
	= str (boost::format ("%s.%s")
	       % (gnumber == gm_singular ? "singular"
		  : gnumber == gm_plural ? "plural"
		  : FAIL (char const *))
	       % (gcase == gm_nominative ? "nominative"
		  : gcase == gm_genitive ? "genitive"
		  : gcase == gm_dative ? "dative"
		  : gcase == gm_accusative ? "accusative"
		  : gcase == gm_instrumental ? "instrumental"
		  : gcase == gm_prepositional ? "prepositional"
		  : gcase == gm_vocative ? "vocative"
		  : FAIL (char const *)));

      data[field].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
