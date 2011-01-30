#ifndef PART_OF_SPEECH_HH
#define PART_OF_SPEECH_HH

#include <string>
#include <AgramtabLib/agramtab_.h>

class part_of_speech
{
  CAgramtab *_m_agramtab;
  int _m_part_of_speech;

public:
  part_of_speech (CAgramtab *agramtab, int pos)
    : _m_agramtab (agramtab)
    , _m_part_of_speech (pos)
  {
  }

  int number () const
  {
    return _m_part_of_speech;
  }

  std::string name () const;
};

#endif//PART_OF_SPEECH_HH
