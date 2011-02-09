#include "pos_handler.hh"

pos_handler::pos_handler (char const *name)
  : _m_name (name)
  , _m_delegate (-1)
{}

pos_handler::pos_handler (int delegate)
  : _m_name (NULL)
  , _m_delegate (delegate)
{}

char const *
pos_handler::template_name () const
{
  return _m_name;
}

int
pos_handler::delegate () const
{
  return _m_delegate;
}

void
pos_handler::fill_hdf (CAgramtab *agramtab,
		       lemmatize::const_iterator const &it,
		       hdf_data_map &data) const
{
  // We should never be called for delegating handler.
  assert (_m_name);
  throw std::runtime_error (std::string ("fill_hdf not implemented for ")
			    + _m_name);
}
