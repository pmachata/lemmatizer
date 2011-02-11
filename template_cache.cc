#include "template_cache.hh"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>
#include <boost/format.hpp>

void
handle_neoerr (NEOERR *err)
{
  if (err != NULL)
    {
      STRING str;
      string_init (&str);
      nerr_error_string (err, &str);
      std::string what = str.buf;
      string_clear (&str);
      throw std::runtime_error (what);
    }
}

template_cache::template_cache ()
  : _m_inotify_fd (inotify_init1 (IN_CLOEXEC | IN_NONBLOCK))
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
	std::cerr << "Warning: Unable to add inotify watch: "
		  << strerror (errno) << "." << std::endl;
      else
	{
	  size_t wd2 = (size_t)wd;
	  // Sanity check: there's not overwhelmingly more watch
	  // descriptors than template IDs.
	  assert (wd2 < 2 * size () || wd2 < 100);
	  if (wd2 >= _m_wd_to_id.size ())
	    _m_wd_to_id.resize (wd2 + 1, -1);
	  _m_wd_to_id[wd] = id;
	  std::cerr << (boost::format ("Inotify: watching %s (%d, wd=%d).\n")
			% path % id % wd);
	}
    }

  CSPARSE *parse;
  handle_neoerr (cs_init (&parse, hdf));
  handle_neoerr (cs_parse_file (parse, template_name));
  (*this)[id] = parse;

  return get (id);
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
	if (len < 0 && errno != EAGAIN)
	  std::cerr << boost::format ("Inotify read: %s\n") % strerror (errno);
	if (len <= 0)
	  break;

	assert (len == sizeof (evt));
	std::cerr << boost::format ("got event: watch=%d\n") % evt.wd;

	int changed_id = -2; // So that we can distinguish untouched
			     // changed_id from uninitialized element
			     // of _m_wd_to_id.
	if (evt.wd < 0
	    || (size_t)evt.wd >= _m_wd_to_id.size ()
	    || (changed_id = _m_wd_to_id[evt.wd]) < 0)
	  std::cerr << (boost::format ("Inotify: strange wd=%d, id=%d.\n")
			% evt.wd % changed_id);

	std::cerr << (boost::format ("Drop template #%d from cache.\n")
		      % changed_id);
	CSPARSE *tmpl = (*this)[changed_id];
	cs_destroy (&tmpl);
	(*this)[changed_id] = NULL;

	// We use IN_ONESHOT, so this wd is not valid anymore.
	_m_wd_to_id[evt.wd] = -1;
      }

  return (*this)[id];
}
