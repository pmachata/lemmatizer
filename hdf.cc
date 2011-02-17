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

#include <wordexp.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <util/neo_err.h>

#include "hdf.hh"
#include "main.hh"

hdf::hdf ()
{
  handle_neoerr (hdf_init (&_m_hdf));
}

hdf::~hdf ()
{
  hdf_destroy (&_m_hdf);
}

hdf::operator HDF* ()
{
  return _m_hdf;
}

HDF *
hdf::node (char const *name)
{
  HDF *node;
  handle_neoerr (hdf_get_node (*this, name, &node));
  return node;
}

namespace
{
  bool
  get_home (boost::filesystem::path &ret)
  {
    wordexp_t we;
    int err = wordexp ("~", &we, WRDE_NOCMD);
    if (err == 0)
      {
	assert (we.we_wordc > 0);
	ret = we.we_wordv[0];
      }

    wordfree (&we);
    if (err != 0)
      std::cerr << "Unknown user home directory." << std::endl;

    return err == 0;
  }

  NEOERR *
  try_read_config (hdf &self, boost::filesystem::path dir)
  {
    dir /= "lemmatizer.hdf";
    NEOERR *ret = hdf_read_file (self, dir.string ().c_str ());
    if (ret == NULL)
      std::cerr << boost::format ("Config loaded from %s\n") % dir;
    return ret;
  }
}

default_hdf::default_hdf ()
{
  // Look for config file in current directory, then in user's home
  // directory and then in system-wide /etc/ directory.

  if (try_read_config (*this, boost::filesystem::current_path ()) == NULL)
    return;

  boost::filesystem::path pth;
  NEOERR *err;
  if (get_home (pth)
      && (err = try_read_config (*this, pth / ".lemmatizer")) == NULL)
    return;

  if ((err = try_read_config (*this, "/etc/")) == NULL)
    return;

  handle_neoerr (err);
}
