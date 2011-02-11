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
pos_handler::fill_hdf (CAgramtab *agramtab,
		       lemmatize::const_iterator const &it,
		       hdf_data_map &data) const
{
  // We should never be called for delegating handler.
  assert (_m_name);
  throw std::runtime_error (std::string ("fill_hdf not implemented for ")
			    + _m_name);
}

pos_handler_map::pos_handler_map ()
{
  pos_handler const *noun = new noun_handler ();
  insert (std::make_pair ((int)pos_noun, noun));
  insert (std::make_pair ((int)pos_pronoun, noun));

  pos_handler const *verb = new verb_handler ();
  insert (std::make_pair ((int)pos_verb, verb));
  insert (std::make_pair ((int)pos_infinitive, verb));
  insert (std::make_pair ((int)pos_adj_participle, verb));
  insert (std::make_pair ((int)pos_adv_participle, verb));

  pos_handler const *adj = new adjective_handler ();
  insert (std::make_pair ((int)pos_adjective, adj));
  insert (std::make_pair ((int)pos_short_adjective, adj));

  pos_handler const *simple = new simple_handler ();
  insert (std::make_pair ((int)pos_adverb, simple));
  insert (std::make_pair ((int)pos_interjection, simple));
  insert (std::make_pair ((int)pos_transition_word, simple));
  insert (std::make_pair ((int)pos_particle, simple));
  insert (std::make_pair ((int)pos_conjunction, simple));
}

pos_handler_map::~pos_handler_map ()
{
  std::set<pos_handler const *> deleted;
  for (iterator it = begin (); it != end (); ++it)
    {
      // Take care not to delete twice.  We update the cache for
      // delegation.
      pos_handler const *to_delete = it->second;
      if (deleted.find (to_delete) == deleted.end ())
	delete to_delete;
      deleted.insert (to_delete);
    }
}

pos_handler const *
pos_handler_map::get_handler (int pos)
{
  pos_handler const *handler = NULL;
  const_iterator it = find (pos);
  if (it == end ())
    return NULL;
  handler = it->second;

  return handler;
}
