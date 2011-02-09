#include "renderer.hh"
#include "fcgi_stdio.h"

#include <iostream>

NEOERR *
fcgi_renderer::render (char *str)
{
  return NULL;
}

stream_renderer::stream_renderer (std::ostream &os)
  : _m_os (os)
{}

NEOERR *
stream_renderer::render (char *str)
{
  _m_os << str;
  return NULL;
}
