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

// Throw std::runtime_error if ERR != NULL.
void handle_neoerr (NEOERR *err);

class template_cache
  : public std::vector<CSPARSE *>
{
  typedef std::vector<CSPARSE *> super_t;

  int _m_inotify_fd;

  // Maps watch descriptors to indices.
  std::vector<int> _m_wd_to_id;

public:
  template_cache ();
  ~template_cache ();

  CSPARSE *add (HDF *hdf, int id, char const *template_name);
  CSPARSE *get (int id);
};

#endif /* _TEMPLATE_CACHE_H_ */
