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
