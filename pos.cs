<?cs def:pos_russian(val) ?><?cs

 # The underlying library doesn't provide meaningful names of parts of
   speech (e.g. verbs are simply "г").  So we have to do it this
   roundabout way. ?><?cs

  if val == "noun" ?>Существительное<?cs else ?><?cs
  if val == "adjective" ?>Прилагательное<?cs else ?><?cs
  if val == "adverb" ?>Наречие<?cs else ?><?cs
  if val == "interjection" ?>Междуметие<?cs else ?><?cs
  if val == "transition_word" ?>Вводное слово<?cs else ?><?cs
  if val == "particle" ?>Частица<?cs else ?><?cs
  if val == "conjunction" ?>Союз<?cs else ?><?cs
  if val == "pronoun" ?>Местоимение<?cs else ?><?cs
  if val == "verb" ?>Глагол<?cs else ?><?cs var:val ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
/def ?>
