<?cs def:pos_russian(val) ?><?cs

 # The underlying library doesn't provide meaningful names of parts of
   speech (e.g. verbs are simply "г").  So we have to do it this
   roundabout way. ?><?cs

  if val == "noun" ?>существительное<?cs else ?><?cs
  if val == "adjective" ?>прилагательное<?cs else ?><?cs
  if val == "adverb" ?>наречие<?cs else ?><?cs
  if val == "interjection" ?>междуметие<?cs else ?><?cs
  if val == "verb" ?>глагол<?cs else ?><?cs var:val ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
/def ?>
