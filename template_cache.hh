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
