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
