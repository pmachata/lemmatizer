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
include:"html.cs" ?>
<h1>Глагол: <?cs var:Form.source ?></h1>

<h2>Свойства</h2>
<table>
<tbody><?cs
call:html_properties(Form)
?><tr><td>инфинитив</td><td><?cs call:out_variants(Form.infinitive) ?></td></tr>
</tbody>
</table><?cs

if:subcount(Form.future) ?>
<h2>Формы будущего времени</h2><?cs
call:conj_table(1, Form.future) ?><?cs
/if ?><?cs

if:subcount(Form.present) ?>
<h2>Формы настоящего времени</h2><?cs
call:conj_table(1, Form.present) ?><?cs
/if ?><?cs

if:subcount(Form.past) ?>
<h2>Формы прошлого времени</h2><?cs
call:conj_table_past(Form.past) ?><?cs
/if ?><?cs

if:subcount(Form.imperative) ?>
<h2>Повелительное наклонение</h2><?cs
call:conj_table(0, Form.imperative) ?><?cs
/if ?><?cs

if:subcount(Form.adj_participle.active.present.positive) ?>
<h2>Действительное причастие настоящего времени</h2><?cs
call:adjective_table(Form.adj_participle.active.present.positive) ?><?cs
/if ?><?cs

if:subcount(Form.adj_participle.active.past.positive) ?>
<h2>Действительное причастие прошлого времени</h2><?cs
call:adjective_table(Form.adj_participle.active.past.positive) ?><?cs
/if ?><?cs

if:subcount(Form.adj_participle.passive.present.positive) ?>
<h2>Страдательное причастие настоящего времени</h2><?cs
call:adjective_table(Form.adj_participle.passive.present.positive) ?><?cs
/if ?><?cs

if:subcount(Form.adj_participle.passive.past.positive) ?>
<h2>Страдательное причастие прошлого времени</h2><?cs
call:adjective_table(Form.adj_participle.passive.past.positive) ?><?cs
/if ?><?cs

if:subcount(Form.adv_participle) ?>
<h2>Деепричастия</h2>

<table>
 <thead>
  <tr><td>время</td><td>форма</td></tr>
 </thead>
 <tbody><?cs
if:subcount(Form.adv_participle.past) ?>
    <tr><td>прошлое</td>
        <td><?cs call:html_variants(Form.adv_participle.past) ?></td></tr><?cs
/if ?><?cs
if:subcount(Form.adv_participle.present) ?>
    <tr><td>настоящее</td>
        <td><?cs call:html_variants(Form.adv_participle.present) ?></td></tr><?cs
/if ?>
 </tbody>
</table><?cs
/if ?>
