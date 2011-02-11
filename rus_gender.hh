#ifndef _RUS_GENDER_H_
#define _RUS_GENDER_H_

#include "rus_gramtab.hh"
#include "gramcodes.ii"
#include <bitset>

enum gender_t
  {
    g_masculine,
    g_feminine,
    g_neuter,
    g__count_genders
  };

extern gram_code_t gender_to_gramcode[g__count_genders];

typedef std::bitset<g__count_genders> gender_set_t;

gender_set_t get_gender (gramcodes const &gramcodes);

#endif /* _RUS_GENDER_H_ */
