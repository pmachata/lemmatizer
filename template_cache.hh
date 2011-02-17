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

#ifndef _TEMPLATE_CACHE_H_
#define _TEMPLATE_CACHE_H_

#include <vector>
#include <cs/cs.h>
#include <util/neo_files.h>

#include "main.hh"

class template_cache
  : public std::vector<CSPARSE *>
{
  typedef std::vector<CSPARSE *> super_t;

  // What follows is here to support cache invalidation.  The whole
  // scheme is that we use inotify to watch the template files that
  // come in.  Each file gets a WD, watch descriptor.  That descriptor
  // is used as an index to _m_wd_to_id.  The ID stored there is used
  // as index to THIS to find the template that needs to be
  // invalidated.
  //
  // Now sometimes we get the same file name for several templates.
  // Then several template IDs will share the same WD.  If that
  // happens, we use _m_collision to store a linked list of all the
  // templates that come from that file.
  int _m_inotify_fd;

  struct wd_to_id_s
  {
    ssize_t id;
    ssize_t next;

    wd_to_id_s () : id (-1), next (-1) {}
  };

  // Maps watch descriptors to indices.  NEXT of slot is an index of
  // next element in _m_collision.
  std::vector<wd_to_id_s> _m_wd_to_id;

  // If one template is shared among several IDs, the collision
  // overflow area is used as a linked list to get other IDs.
  std::vector<wd_to_id_s> _m_collision;

  // First free slot in _m_collision.  NEXT of that slot points to
  // next free slot.  -1 for no free slot.
  ssize_t _m_collision_free;

  ssize_t release (wd_to_id_s &slot, ssize_t next = -1);

public:
  template_cache ();
  ~template_cache ();

  CSPARSE *add (HDF *hdf, int id, char const *template_name);
  CSPARSE *get (int id);
};

#endif /* _TEMPLATE_CACHE_H_ */
