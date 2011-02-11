#include "rus_gender.hh"
#include "gramcodes.hh"
#include <vector>

gram_code_t gender_to_gramcode[g__count_genders] = {
  gm_masculine,
  gm_feminine,
  gm_neuter
};

gender_set_t
get_gender (gramcodes const &ac)
{
  std::vector<grammeme> grammemes = ac.grammemes ();
  gender_set_t ret;

  for (std::vector<grammeme>::const_iterator gt = grammemes.begin ();
       gt != grammemes.end (); ++gt)
    switch (gt->value ())
      {
      case gm_masculine:
	ret[g_masculine] = true;
	continue;
      case gm_feminine:
	ret[g_feminine] = true;
	continue;
      case gm_neuter:
	ret[g_neuter] = true;
	continue;
      case gm_masc_femin:
	ret[g_masculine] = true;
	ret[g_feminine] = true;
	continue;
      }

  return ret;
}
