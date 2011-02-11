#include "pos_handler.hh"
#include "adjective.hh"
#include "noun.hh"
#include "simple.hh"
#include "verb.hh"

pos_handler::pos_handler (char const *name)
  : _m_name (name)
  , _m_delegate (-1)
{}

pos_handler::pos_handler (int delegate)
  : _m_name (NULL)
  , _m_delegate (delegate)
{}

char const *
pos_handler::template_name () const
{
  return _m_name;
}

int
pos_handler::delegate () const
{
  return _m_delegate;
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
  insert (std::make_pair ((int)pos_noun,
			  new noun_handler ()));

  insert (std::make_pair ((int)pos_verb,
			  new verb_handler ()));

  insert (std::make_pair ((int)pos_adjective,
			  new adjective_handler ()));

  insert (std::make_pair ((int)pos_short_adjective,
			  new short_adjective_handler ()));

  // We can share handlers.
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
  int orig_pos = pos;
  int delegated_pos;
  pos_handler const *handler = NULL;
  while (true)
    {
      const_iterator it = find (pos);
      if (it == end ())
	return NULL;
      handler = it->second;
      delegated_pos = pos;
      pos = handler->delegate ();
      if (pos == -1)
	break;

      // Guard against infinite cycle.
      if (pos == orig_pos)
	return NULL;
    }
  assert (handler != NULL);

  // Update the cache so that we don't have to go through delegation
  // loops next time around.
  if (delegated_pos != orig_pos)
    {
      delete (*this)[orig_pos];
      (*this)[orig_pos] = handler;
    }

  return handler;
}
