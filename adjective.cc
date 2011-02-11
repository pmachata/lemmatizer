#include "adjective.hh"
#include "forms.hh"
#include "format.hh"

#include <sstream>

adjective_handler::adjective_handler ()
  : pos_handler ("adjective")
{}

std::string
adjective_handler::format_form_key (gram_code_t degree, gram_code_t gcase,
				    gram_code_t number, gram_code_t gender,
				    bool animate, bool inanimate,
				    bool secondary)
{
  if (degree == gm__invalid)
    degree = gm_positive;
  bool is_short = gcase == gm__invalid && degree == gm_positive;

  std::ostringstream os;
  os << format_rus (degree);

  if (degree == gm_comparative)
    {
      if (secondary)
	os << '.' << format_rus (gm_secondary);
      else
	os << ".primary";
    }

  if (is_short)
    os << ".short";

  if (number != gm__invalid)
    {
      if (gender != gm__invalid)
	{
	  assert (number == gm_singular);
	  os << '.' << format_rus (gender);
	}
      else
	{
	  assert (number == gm_plural);
	  os << '.' << format_rus (number);
	}
    }

  if (gcase != gm__invalid)
    os << '.' << format_rus (gcase);

  if ((animate || inanimate) && !(animate && inanimate))
    os << '.' << format_rus (animate ? gm_animate : gm_inanimate);

  return os.str ();
}

void
adjective_handler::fill_hdf (CAgramtab *agramtab,
			     lemmatize::const_iterator const &it,
			     hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();
  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    {
      gramcodes ac = ft.ancode ();

      gram_code_t gnumber = gm__invalid;
      gram_code_t gcase = gm__invalid;
      gram_code_t gender = gm__invalid;
      bool animate = false;
      bool inanimate = false;
      gram_code_t degree = gm__invalid;
      bool secondary = false;

      std::vector<grammeme> grammemes = ac.grammemes ();
      for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
	   gt != grammemes.end (); ++gt)
	{
	  gram_code_t code = gt->value_as<gram_code_t> ();
	  if (!extract_rus_number (gnumber, code)
	      && !extract_rus_case (gcase, code)
	      && !extract_rus_gender (gender, code)
	      && !extract_rus_adj_degree (degree, code))
	    {
	      if (code == gm_animate)
		animate = true;
	      else if (code == gm_inanimate)
		inanimate = true;
	      else if (code == gm_secondary)
		secondary = true;
	    }
	}

      if (gnumber == gm__invalid && degree == gm__invalid)
	throw std::runtime_error
	  ("If number is undefined, then degree must be defined.");

      if (degree == gm__invalid && secondary)
	throw std::runtime_error
	  ("Don't know what 'secondary' means without degree.");

      std::string key = format_form_key (degree, gcase, gnumber, gender,
					 animate, inanimate, secondary);
      data[key].push_back (std::make_pair (*ft, ft.accent ()));
    }
}
