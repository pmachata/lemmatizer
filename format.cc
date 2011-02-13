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

#include <boost/format.hpp>
#include "rus_gramtab.hh"

char const *
format_rus (pos_code_t val)
{
  switch (val)
    {
    case pos_noun:
      return "noun";
    case pos_adjective:
      return "adjective";
    case pos_verb:
      return "verb";
    case pos_adverb:
      return "adverb";
    case pos_interjection:
      return "interjection";
    case pos_transition_word:
      return "transition_word";
    case pos_particle:
      return "particle";
    case pos_conjunction:
      return "conjunction";
    case pos_pronoun:
      return "pronoun";
    case pos_short_adjective:
      return "short_adjective";
    case pos_infinitive:
      return "infinitive";
    case pos_adv_participle:
      return "adv_participle";
    case pos_adj_participle:
      return "adj_participle";

    case pos_4:
    case pos_5:
    case pos_6:
    case pos_7:
    case pos_9:
    case pos_10:
    case pos_11:
    case pos_15:
    case pos_short_participle:
    case pos__invalid:
      break;
  };

  throw std::runtime_error
    (str (boost::format ("Cannot determine how to call part of speech %d.")
	  % val));
}

char const *
format_rus (gram_code_t val)
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
    case gm_colloquial:
      return "colloquial";
    case gm_indeclinable:
      return "indeclinable";

    case gm_25:
    case gm_26:
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
