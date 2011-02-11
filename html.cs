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


def:begin_html(word) ?><html>
<head>
    <title>Лемматизатор: <?cs var:word ?></title>
    <link rel="stylesheet" type="text/css" href="style.css" />
</head>
<body>
<?cs
/def ?><?cs

def:end_html () ?></body></html><?cs
/def ?><?cs

def:conj_table (all, sub) ?><table>
<thead>
<tr><td>&nbsp;</td><td>ед</td><td>мн</td></tr>
</thead>

<tbody><?cs
if:all || subcount(sub.singular.1st_person) || subcount(sub.plural.1st_person) ?>
<tr><td>1.л.</td>
    <td><?cs call:html_variants(sub.singular.1st_person) ?></td>
    <td><?cs call:html_variants(sub.plural.1st_person) ?></td></tr><?cs
/if ?><?cs

if:all || subcount(sub.singular.2nd_person) || subcount(sub.plural.2nd_person) ?>
<tr><td>2.л.</td>
    <td><?cs call:html_variants(sub.singular.2nd_person) ?></td>
    <td><?cs call:html_variants(sub.plural.2nd_person) ?></td></tr><?cs
/if ?><?cs

if:all || subcount(sub.singular.3rd_person) || subcount(sub.plural.3rd_person) ?>
<tr><td>3.л.</td>
    <td><?cs call:html_variants(sub.singular.3rd_person) ?></td>
    <td><?cs call:html_variants(sub.plural.3rd_person) ?></td></tr><?cs
/if ?>
</tbody>

</table><?cs
/def ?><?cs

def:conj_table_past (sub) ?><table>
<thead>
<tr><td>&nbsp;</td><td>ед</td><td>мн</td></tr>
</thead>

<tbody>
<tr><td>м.</td>
    <td><?cs call:html_variants(sub.masculine) ?></td>
    <td rowspan="3"><?cs call:html_variants(sub.plural) ?></td></tr>

<tr><td>ж.</td>
    <td><?cs call:html_variants(sub.feminine) ?></td></tr>

<tr><td>с.</td>
    <td><?cs call:html_variants(sub.neuter) ?></td></tr>
</tbody>

</table><?cs
/def ?><?cs

def:adjective_table (sub) ?><table>
<thead>
<tr><td rowspan="2">&nbsp;</td><td colspan="3">ед.ч.</td><td rowspan="2">мн.ч.</td></tr>
<tr><td>мр</td><td>жр</td><td>ср</td></tr>
</thead>

<tbody>

<tr><td>им.</td>
    <td><?cs call:html_variants(sub.masculine.nominative) ?></td>
    <td><?cs call:html_variants(sub.feminine.nominative) ?></td>
    <td><?cs call:html_variants(sub.neuter.nominative) ?></td>
    <td><?cs call:html_variants(sub.plural.nominative) ?></td></tr>

<tr><td>р.</td>
    <td><?cs call:html_variants(sub.masculine.genitive) ?></td>
    <td><?cs call:html_variants(sub.feminine.genitive) ?></td>
    <td><?cs call:html_variants(sub.neuter.genitive) ?></td>
    <td><?cs call:html_variants(sub.plural.genitive) ?></td></tr>

<tr><td>д.</td>
    <td><?cs call:html_variants(sub.masculine.dative) ?></td>
    <td><?cs call:html_variants(sub.feminine.dative) ?></td>
    <td><?cs call:html_variants(sub.neuter.dative) ?></td>
    <td><?cs call:html_variants(sub.plural.dative) ?></td></tr>

<tr><td>в.од.</td>
    <td><?cs call:html_variants(sub.masculine.accusative.animate) ?></td>
    <td><?cs call:html_variants(sub.feminine.accusative) ?></td>
    <td><?cs call:html_variants(sub.neuter.accusative) ?></td>
    <td><?cs call:html_variants(sub.plural.accusative.animate) ?></td></tr>

<tr><td>в.но.</td>
    <td><?cs call:html_variants(sub.masculine.accusative.inanimate) ?></td>
    <td><?cs call:html_variants(sub.feminine.accusative) ?></td>
    <td><?cs call:html_variants(sub.neuter.accusative) ?></td>
    <td><?cs call:html_variants(sub.plural.accusative.inanimate) ?></td></tr>

<tr><td>тв.</td>
    <td><?cs call:html_variants(sub.masculine.instrumental) ?></td>
    <td><?cs call:html_variants(sub.feminine.instrumental) ?></td>
    <td><?cs call:html_variants(sub.neuter.instrumental) ?></td>
    <td><?cs call:html_variants(sub.plural.instrumental) ?></td></tr>

<tr><td>пр.</td>
    <td><?cs call:html_variants(sub.masculine.prepositional) ?></td>
    <td><?cs call:html_variants(sub.feminine.prepositional) ?></td>
    <td><?cs call:html_variants(sub.neuter.prepositional) ?></td>
    <td><?cs call:html_variants(sub.plural.prepositional) ?></td></tr><?cs

if:subcount(sub.short) ?>
<tr><td>к.ф.</td>
    <td><?cs call:html_variants(sub.short.masculine) ?></td>
    <td><?cs call:html_variants(sub.short.feminine) ?></td>
    <td><?cs call:html_variants(sub.short.neuter) ?></td>
    <td><?cs call:html_variants(sub.short.plural) ?></td></tr><?cs
/if ?>
</tbody>
</table><?cs

/def ?>
