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

def:out_variants(val) ?><?cs
  each:item = val ?><?cs
    if !first(item) ?>,<?cs /if ?><?cs
    var:item ?><?cs
  /each ?><?cs
/def ?><?cs

def:html_variants(val) ?><?cs
  each:item = val ?><?cs
    if !first(item) ?><br/><?cs /if ?><?cs
    var:item ?><?cs
  /each ?><?cs
/def ?>
