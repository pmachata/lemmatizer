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

#include "template_cache.hh"
#include "main.hh"

#include <boost/format.hpp>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>

template_cache::template_cache ()
  : _m_inotify_fd (inotify_init1 (IN_CLOEXEC | IN_NONBLOCK))
  , _m_collision_free (-1)
{
  // Warn, but keep running.  We won't be able to check for updates to
  // templates, but otherwise it will keep working.
  if (_m_inotify_fd < 0)
    std::cerr << (boost::format ("Warning: Unable to initialize inotify: %s.\n")
		  % strerror (errno));
}

template_cache::~template_cache ()
{
  for (const_iterator it = begin (); it != end (); ++it)
    {
      CSPARSE *parse = *it;
      if (parse != NULL)
	cs_destroy (&parse);
    }

  while (close (_m_inotify_fd) != 0)
    if (errno != EINTR)
      throw std::runtime_error (strerror (errno));
}

CSPARSE *
template_cache::add (HDF *hdf, int id, char const *template_name)
{
  assert (id >= 0);
  if ((size_t)id >= size ())
    resize ((size_t)id + 1);
  assert (at (id) == NULL);

  int wd = -1;
  if (_m_inotify_fd >= 0)
    {
      char path[_POSIX_PATH_MAX];
      handle_neoerr (hdf_search_path (hdf, template_name, path));
      wd = inotify_add_watch(_m_inotify_fd, path,
			     IN_DELETE_SELF | IN_MODIFY
			     | IN_MOVE_SELF | IN_ONESHOT);
      if (wd < 0)
	std::cerr << (boost::format
		      ("Warning: Unable to add inotify watch: %s.\n")
		      % strerror (errno));
      else
	{
	  size_t wd2 = (size_t)wd;
	  // Sanity check: there's not overwhelmingly more watch
	  // descriptors than template IDs.
	  assert (wd2 < 2 * size () || wd2 < 100);
	  if (wd2 >= _m_wd_to_id.size ())
	    _m_wd_to_id.resize (wd2 + 1);

#ifdef LOG
	  std::cerr << (boost::format ("Inotify: watching %s (%d, wd=%d).\n")
			% path % id % wd);
#endif

	  // If this slot is already used, it means that the template
	  // is used under more than one ID.
	  if (_m_wd_to_id[wd].id == -1)
	    _m_wd_to_id[wd].id = id;
	  else
	    {
	      // Find first free collision node.
	      if (_m_collision_free == -1)
		{
		  _m_collision.push_back (wd_to_id_s ());
		  _m_collision_free = _m_collision.size () - 1;
		}
	      ssize_t slot_n = _m_collision_free;
	      wd_to_id_s &slot = _m_collision[slot_n];
	      assert (slot.id == -1);
	      _m_collision_free = slot.next;

	      slot.next = _m_wd_to_id[wd].next;
	      _m_wd_to_id[wd].next = slot_n;
	      slot.id = id;
	    }

	  std::ostringstream os;
	  os << _m_wd_to_id[wd].id;
	  for (ssize_t i = _m_wd_to_id[wd].next; i != -1;
	       i = _m_collision[i].next)
	    os << boost::format (",(%d)%d") % i % _m_collision[i].id;
#ifdef LOG
	  std::cerr << boost::format ("wd=%d now refers to templates %s\n")
	    % wd % os.str ();
#endif
	}
    }

  CSPARSE *parse;
  handle_neoerr (cs_init (&parse, hdf));
  handle_neoerr (cs_parse_file (parse, template_name));
  (*this)[id] = parse;

  return get (id);
}

ssize_t
template_cache::release (wd_to_id_s &slot, ssize_t next)
{
#ifdef LOG
  std::cerr << (boost::format ("Drop template #%d from cache.\n")
		% slot.id);
#endif
  CSPARSE *tmpl = (*this)[slot.id];
  cs_destroy (&tmpl);
  (*this)[slot.id] = NULL;

  // We use IN_ONESHOT, so this wd is not valid anymore.
  slot.id = -1;
  ssize_t ret = slot.next;
  slot.next = next;
  return ret;
}


CSPARSE *
template_cache::get (int id)
{
  if (id < 0 || (size_t)id >= size ())
    return NULL;

  if (_m_inotify_fd >= 0)
    while (true)
      {
	// We don't need inotify_event::name, so the struct itself
	// is big enough buffer.
	inotify_event evt;
	ssize_t len;
	do
	  len = read (_m_inotify_fd, &evt, sizeof (evt));
	while (len < 0 && errno == EINTR);

	// EAGAIN: The read would block, i.e. no data is ready.
	// There's no need to warn about this.
#ifdef LOG
	if (len < 0 && errno != EAGAIN)
	  std::cerr << boost::format ("Inotify read: %s\n") % strerror (errno);
#endif
	if (len <= 0)
	  break;

	assert (len == sizeof (evt));

	if (evt.wd < 0 || (size_t)evt.wd >= _m_wd_to_id.size ())
	  {
	    std::cerr << boost::format ("Inotify: strange wd=%d.\n") % evt.wd;
	    continue;
	  }

	int changed_id = _m_wd_to_id[evt.wd].id;
	if (changed_id < 0)
	  {
	    // This must be a second change of a file that we already
	    // invalidated.
#ifdef LOG
	    std::cerr << (boost::format ("File #%d changed again.\n")
			  % changed_id);
#endif
	    continue;
	  }

	// Return the whole chain to free store.
	for (ssize_t i = release (_m_wd_to_id[evt.wd]); i != -1; )
	  {
	    ssize_t next = release (_m_collision[i], _m_collision_free);
	    _m_collision_free = i;
	    i = next;
	  }

	std::ostringstream os;
	for (ssize_t i = _m_collision_free; i != -1;
	     i = _m_collision[i].next)
	  os << "," << i;
#ifdef LOG
	std::cerr << boost::format ("Free chain is now %s\n") % os.str ();
#endif
      }

  return (*this)[id];
}
