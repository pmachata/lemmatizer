<?cs
include:"header.cs" ?><?cs
include:"html.cs" ?><?cs

call:begin_html(Form.positive.masculine.nominative.0) ?>

<h1>Прилагательное: <?cs var:Form.positive.masculine.nominative.0 ?></h1><?cs

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
