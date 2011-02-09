#include <boost/format.hpp>
#include "rus_gramtab.hh"

char const *
format_rus_grammeme_value (gram_code_t val)
{
  switch (val)
    {
    case gm_plural:
      return "plural";
    case gm_singular:
      return "singular";
    case gm_nominative:
      return "nominative";
    case gm_genitive:
      return "genitive";
    case gm_dative:
      return "dative";
    case gm_accusative:
      return "accusative";
    case gm_instrumental:
      return "instrumental";
    case gm_prepositional:
      return "prepositional";
    case gm_vocative:
      return "vocative";
    case gm_masculine:
      return "masculine";
    case gm_feminine:
      return "feminine";
    case gm_neuter:
      return "neuter";
    case gm_masc_femin:
      return "masculine/feminine";
    case gm_present:
      return "present";
    case gm_future:
      return "future";
    case gm_past:
      return "past";
    case gm_1st_person:
      return "1st_person";
    case gm_2nd_person:
      return "2nd_person";
    case gm_3rd_person:
      return "3rd_person";
    case gm_imperative:
      return "imperative";
    case gm_animate:
      return "animate";
    case gm_inanimate:
      return "inanimate";
    case gm_comparative:
      return "comparative";
    case gm_perfective:
      return "perfective";
    case gm_imperfective:
      return "imperfective";
    case gm_active:
      return "active";
    case gm_passive:
      return "passive";
    case gm_superlative:
      return "superlative";
    case gm_positive:
      return "positive";
    case gm_secondary:
      return "secondary";

    case gm_25:
    case gm_26:
    case gm_0:
    case gm_abbr:
    case gm_patronymic:
    case gm_32:
    case gm_33:
    case gm_34:
    case gm_35:
    case gm_36:
    case gm_37:
    case gm_first_name:
    case gm_surname:
    case gm_impersonal:
    case gm_jargon:
    case gm_42:
    case gm_colloquial:
    case gm_44:
    case gm_45:
    case gm_poetic:
    case gm_prof:
    case gm__invalid:
      break;
    };

  throw std::runtime_error
    (str (boost::format ("Cannot determine how to call grammeme value %d.")
	  % val));
}
