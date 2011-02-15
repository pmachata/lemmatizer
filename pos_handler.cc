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

#include "pos_handler.hh"
#include "adjective.hh"
#include "noun.hh"
#include "simple.hh"
#include "verb.hh"

pos_handler::pos_handler (char const *name)
  : _m_name (name)
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

pos_handler_map::pos_handler_map ()
{
  pos_handler const *noun = new noun_handler ();
  insert (pos_noun, noun);
  insert (pos_pronoun, noun);

  pos_handler const *verb = new verb_handler ();
  insert (pos_verb, verb);
  insert (pos_infinitive, verb);
  insert (pos_adj_participle, verb);
  insert (pos_adv_participle, verb);

  pos_handler const *adj = new adjective_handler ();
  insert (pos_adjective, adj);
  insert (pos_short_adjective, adj);
  insert (pos_pronominal_adjective, adj);

  pos_handler const *simple = new simple_handler ();
  insert (pos_adverb, simple);
  insert (pos_interjection, simple);
  insert (pos_transition_word, simple);
  insert (pos_particle, simple);
  insert (pos_conjunction, simple);
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
}

pos_handler const *
pos_handler_map::get_handler (int pos)
{
  if (pos < 0 || (size_t)pos >= size ())
    return NULL;
  return operator[] (pos);
}
