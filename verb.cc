#include "verb.hh"
#include "forms.hh"
#include "rus_gramtab.hh"
#include "format.hh"
#include "adjective.hh"

#include <boost/format.hpp>
#include <iostream>

verb_handler::verb_handler ()
  : pos_handler ("verb")
{
}

void
verb_handler::fill_hdf (CAgramtab *agramtab,
			lemmatize::const_iterator const &it,
			hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      std::string category;
      std::vector<grammeme> gs = ft.ancode ().grammemes ();

      gram_code_t number = gm__invalid;
      gram_code_t voice = gm__invalid;
      gram_code_t gender = gm__invalid;
      gram_code_t tense = gm__invalid;
      gram_code_t person = gm__invalid;
      gram_code_t gcase = gm__invalid;
      bool imperative = false;
      bool animate = false;
      bool inanimate = false;

      std::vector<gram_code_t> extra;
      for (std::vector<grammeme>::const_iterator gt = gs.begin ();
	   gt != gs.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  if (!extract_rus_number (number, code)
	      && !extract_rus_voice (voice, code)
	      && !extract_rus_gender (gender, code)
	      && !extract_rus_tense (tense, code)
	      && !extract_rus_person (person, code)
	      && !extract_rus_case (gcase, code))
	    {
	      if (code == gm_imperative)
		imperative = true;
	      else if (code == gm_animate)
		animate = true;
	      else if (code == gm_inanimate)
		inanimate = true;
	      else if (code == gm_colloquial)
		{
		  extra.push_back (code);
		  continue;
		}
	      else
		std::cout << (boost::format ("Unhandled grammeme %s\n")
			      % format_rus (code));
	    }
	  if (category.length () > 0)
	    category += ",";
	  category += gt->c_str ();
	}

      std::string key;

      // Forms with animate and/or inanimate attributes are adjectival
      // participles.
      if (animate || inanimate)
	{
	  std::string sub_key = adjective_handler::format_form_key
	    (gm_positive, gcase, number, gender, animate, inanimate, false);
	  key = str (boost::format ("adj_participle.%s.%s.%s")
		     % format_rus (voice)
		     % format_rus (tense)
		     % sub_key);
	}

      else if (tense == gm__invalid)
	{
	  if (imperative)
	    key = str (boost::format ("%s.%s.%s")
		       % format_rus (gm_imperative)
		       % format_rus (number)
		       % format_rus (person));
	  else
	    key = "infinitive";
	}

      // These are adverbial participles.
      else if (number == gm__invalid)
	key = str (boost::format ("adv_participle.%s")
		   % format_rus (tense));

      else if (tense == gm_future || tense == gm_present)
	key = str (boost::format ("%s.%s.%s")
		   % format_rus (tense)
		   % format_rus (number)
		   % format_rus (person));

      // past tense
      else
	{
	  if (number == gm_singular)
	    key = str (boost::format ("%s.%s")
		       % format_rus (tense)
		       % format_rus (gender));
	  else
	    key = str (boost::format ("%s.%s")
		       % format_rus (tense)
		       % format_rus (number));
	}

      data[key].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
