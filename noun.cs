<?cs
include:"header.cs" ?><?cs
include:"html.cs" ?><?cs

call:begin_html(Form.singular.nominative.0) ?>

<h1>Существительное: <?cs var:Form.singular.nominative.0 ?></h1>

<h2>Свойства</h2>

<table>
<thead>
<tr><td>род</td></tr>
</thead>
<tbody>
<tr><td><?cs call:out_variants(Form.gender) ?></td></tr>
</tbody>
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
    <td><?cs call:html_variants(Form.plural.prepositional) ?></td></tr>
</tbody>

</table><?cs

call:end_html() ?>
