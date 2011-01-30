#include "part_of_speech.hh"

std::string
part_of_speech::name () const
{
  return _m_agramtab->GetPartOfSpeechStr(_m_part_of_speech);
}
