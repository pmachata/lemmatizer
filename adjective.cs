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

call:begin_html(Form.positive.masculine.nominative.0) ?>

<h1>Прилагательное: <?cs var:Form.positive.masculine.nominative.0 ?></h1>

<h2>Свойства</h2>
<table>
<tbody><?cs
call:html_properties(Form)
?></tbody>
</table><?cs

if:subcount(Form.positive) ?>
<h2>Положительная степень</h2><?cs
call:adjective_table(Form.positive) ?><?cs
/if

?><?cs

if:subcount(Form.comparative) ?>
<h2>Сравнительная степень</h2>

<table>
<thead>
<tr><td>&nbsp;</td><td>срав.</td></tr>
</thead>
<tbody><?cs

if:subcount(Form.comparative.primary) ?>
<tr><td>1</td><td><?cs call:html_variants(Form.comparative.primary) ?></td></tr><?cs
/if ?><?cs

if:subcount(Form.comparative.secondary) ?>
<tr><td>2</td><td><?cs call:html_variants(Form.comparative.secondary) ?></td></tr><?cs
/if ?>
</tbody>
</table><?cs
/if ?><?cs

if:subcount(Form.superlative) ?>
<h2>Превосходная степень</h2><?cs
call:adjective_table(Form.superlative) ?><?cs
/if

?>
