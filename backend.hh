#ifndef BACKEND_HH
#define BACKEND_HH

#include <iosfwd>

class backend
{
public:
  virtual ~backend () {}
  virtual bool get_word (std::string &word) = 0;
  virtual bool render (char const *str) = 0;
  virtual void before_render () {}
  virtual void after_render () {}
};

class console_backend
  : public backend
{
  std::istream &_m_is;
  std::ostream &_m_os;

public:
  console_backend (std::istream &is, std::ostream &os);
  virtual bool get_word (std::string &word);
  virtual bool render (char const *str);
void
before_render ()
{
  render ("Content-type: text/html\n\n");
}

};

#endif//BACKEND_HH
