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
