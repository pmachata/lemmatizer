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
include:"pos.cs" ?><?cs

call:begin_html(Form.word.0) ?>

<h1><?cs call:pos_russian(Form.pos.0) ?>: <?cs var:Form.word.0 ?></h1><?cs

if:subcount(Form.word)>1 ?>
<table>
<tbody>
<tr><td>слово</td><td><?cs call:out_variants(Form.word) ?></td></tr>
</tbody>
</table><?cs
/if ?>
