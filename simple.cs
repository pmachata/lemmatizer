<?cs
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
