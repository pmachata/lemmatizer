<?cs
# Copyright (C) 2011 Petr Machata
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>. ?><?cs

def:pos_russian(val) ?><?cs

 # The underlying library doesn't provide meaningful names of parts of
   speech (e.g. verbs are simply "г").  So we have to do it this
   roundabout way. ?><?cs

  if val == "noun" ?>Существительное<?cs else ?><?cs
  if val == "adjective" ?>Прилагательное<?cs else ?><?cs
  if val == "short_adjective" ?>Прилагательное<?cs else ?><?cs
  if val == "adverb" ?>Наречие<?cs else ?><?cs
  if val == "interjection" ?>Междуметие<?cs else ?><?cs
  if val == "transition_word" ?>Вводное слово<?cs else ?><?cs
  if val == "particle" ?>Частица<?cs else ?><?cs
  if val == "conjunction" ?>Союз<?cs else ?><?cs
  if val == "pronoun" ?>Местоимение<?cs else ?><?cs
  if val == "number" ?>Числительное<?cs else ?><?cs
  if val == "ordinal_number" ?>Порядковое числительное<?cs else ?><?cs
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
  /if ?><?cs
  /if ?><?cs
  /if ?><?cs
/def ?>
