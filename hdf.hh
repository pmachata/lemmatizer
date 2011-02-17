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

#ifndef _HDF_H_
#define _HDF_H_

#include <cs/cs.h>

class hdf
{
  HDF *_m_hdf;

public:
  hdf ();
  ~hdf ();

  operator HDF* ();

  // This doesn't return HDF, because ~hdf unconditionally destroys
  // the underlying _m_hdf.  This is a poor man's wrapper.
  HDF *node (char const *name);
};

class default_hdf
  : public hdf
{
public:
  default_hdf ();
};

#endif /* _HDF_H_ */
