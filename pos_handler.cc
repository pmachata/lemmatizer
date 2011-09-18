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

#include "adjective.hh"
#include "default.hh"
#include "noun.hh"
#include "number.hh"
#include "pos_handler.hh"
#include "simple.hh"
#include "verb.hh"

pos_handler::pos_handler (id_allocator &parent, char const *name)
  : _m_name (name)
  , _m_id (parent.get_id ())
{}

char const *
pos_handler::template_name () const
{
  return _m_name;
}


void
pos_handler_map::insert (int pos, pos_handler const *handler)
{
  assert (pos >= 0);
  size_t idx = (size_t)pos;
  if (idx >= size ())
    resize (idx + 1, NULL);
  (*this)[idx] = handler;
}

pos_handler_map::pos_handler_map (id_allocator &id_a)
  : _m_default (new default_handler (id_a))
{
  pos_handler const *noun = new noun_handler (id_a);
  insert (pos_noun, noun);
  insert (pos_pronoun, noun);

  pos_handler const *verb = new verb_handler (id_a);
  insert (pos_verb, verb);
  insert (pos_infinitive, verb);
  insert (pos_adj_participle, verb);
  insert (pos_adv_participle, verb);
  insert (pos_short_participle, verb);

  pos_handler const *adj = new adjective_handler (id_a);
  insert (pos_adjective, adj);
  insert (pos_short_adjective, adj);
  insert (pos_pronominal_adjective, adj);
  insert (pos_ordinal_number, adj);

  pos_handler const *simple = new simple_handler (id_a);
  insert (pos_adverb, simple);
  insert (pos_interjection, simple);
  insert (pos_transition_word, simple);
  insert (pos_particle, simple);
  insert (pos_conjunction, simple);

  pos_handler const *number = new number_handler (id_a);
  insert (pos_number, number);
}

pos_handler_map::~pos_handler_map ()
{
  // Take care not to delete twice, we share handlers.
  std::sort (begin (), end ());
  pos_handler const *last = NULL;
  for (iterator it = begin (); it != end (); ++it)
    {
      if (*it != last)
	delete *it;
      last = *it;
    }

  delete _m_default;
}

pos_handler const *
pos_handler_map::get_handler (int pos)
{
  if (pos < 0 || (size_t)pos >= size ())
    return NULL;

  if (pos_handler const *ret = operator[] (pos))
    return ret;
  else
    return _m_default;
}
