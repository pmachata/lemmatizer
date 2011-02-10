#ifndef FCGI_BACKEND_HH
#define FCGI_BACKEND_HH

#include "backend.hh"

class fcgi_backend
  : public backend
{
public:
  virtual bool get_word (std::string &word);
  virtual void before_render ();
  virtual bool render (char const *str);
};

#endif//FCGI_BACKEND_HH
