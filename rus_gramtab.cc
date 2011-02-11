#include "rus_gramtab.hh"

bool
extract_rus_number (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_plural:
    case gm_singular:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_case (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_nominative:
    case gm_genitive:
    case gm_dative:
    case gm_accusative:
    case gm_instrumental:
    case gm_prepositional:
    case gm_vocative:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_gender (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_masculine:
    case gm_feminine:
    case gm_neuter:
    case gm_masc_femin:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_adj_degree (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_positive:
    case gm_comparative:
    case gm_superlative:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_voice (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_active:
    case gm_passive:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_animacy (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_animate:
    case gm_inanimate:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_tense (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_past:
    case gm_present:
    case gm_future:
      ret = code;
      return true;

    default:
      return false;
    }
}

bool
extract_rus_person (gram_code_t &ret, gram_code_t code)
{
  switch (code)
    {
    case gm_1st_person:
    case gm_2nd_person:
    case gm_3rd_person:
      //case gm_impersonal: XXX ???
      ret = code;
      return true;

    default:
      return false;
    }
}
