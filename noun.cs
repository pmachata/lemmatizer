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
call:html_properties(Form)
?></tbody>
</table>

<h2>Таблица склонений</h2>

<table>
<thead>
<tr><td>&nbsp;</td><td>ед.ч.</td><td>мн.ч.</td></tr>
</thead>

<tbody>
<tr><td>им.</td>
    <td><?cs call:html_variants(Form.singular.nominative) ?></td>
    <td><?cs call:html_variants(Form.plural.nominative) ?></td></tr>

<tr><td>р.</td>
    <td><?cs call:html_variants(Form.singular.genitive) ?></td>
    <td><?cs call:html_variants(Form.plural.genitive) ?></td></tr>

<tr><td>д.</td>
    <td><?cs call:html_variants(Form.singular.dative) ?></td>
    <td><?cs call:html_variants(Form.plural.dative) ?></td></tr>

<tr><td>в.</td>
    <td><?cs call:html_variants(Form.singular.accusative) ?></td>
    <td><?cs call:html_variants(Form.plural.accusative) ?></td></tr>

<tr><td>тв.</td>
    <td><?cs call:html_variants(Form.singular.instrumental) ?></td>
    <td><?cs call:html_variants(Form.plural.instrumental) ?></td></tr>

<tr><td>пр.</td>
    <td><?cs call:html_variants(Form.singular.prepositional) ?></td>
    <td><?cs call:html_variants(Form.plural.prepositional) ?></td></tr><?cs

if:subcount(Form.singular.vocative) ?>

<tr><td>зв.</td>
    <td><?cs call:html_variants(Form.singular.vocative) ?></td>
    <td><?cs call:html_variants(Form.plural.vocative) ?></td></tr><?cs
/if ?>
</tbody>

</table>
