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

include:"header.cs" ?><?cs
include:"html.cs" ?><?cs
include:"pos.cs" ?>
<h1><?cs call:pos_russian(Form.pos) ?>: <?cs var:Form.source ?></h1>

<h2>Свойства</h2>

<table>
<tbody><?cs
call:html_properties(Form) ?><?cs
  each:item = Form.forms ?>
    <tr><td><?cs
    call:out_variants(item.attributes) ?></td><td><?cs
    var:item ?></td></tr><?cs
  /each ?>
</tbody>
</table>
