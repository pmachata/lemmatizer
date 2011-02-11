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
