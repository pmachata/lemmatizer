#ifndef RENDERER_HH
#define RENDERER_HH

#include <iosfwd>
#include <util/neo_err.h>

class renderer
{
public:
  virtual ~renderer () {}
  virtual NEOERR *render (char *str) = 0;
};

class fcgi_renderer
  : public renderer
{
public:
  virtual NEOERR *render (char *str);
};

class stream_renderer
  : public renderer
{
  std::ostream &_m_os;

public:
  stream_renderer (std::ostream &os);
  virtual NEOERR *render (char *str);
};

#endif//RENDERER_HH
