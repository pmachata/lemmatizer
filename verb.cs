<?cs
include:"header.cs" ?><?cs
include:"html.cs" ?><?cs

call:begin_html(Form.infinitive.0) ?>

<h1>Глагол: <?cs var:Form.infinitive.0 ?></h1>

<h2>Свойства</h2>

<table>
<tr><td>found</td><td><?cs if:Form.found ?>yes<?cs else ?>no<?cs /if ?></td></tr>
<tr><td>инфинитив</td><td><?cs call:out_variants(Form.infinitive) ?></td></tr>
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
/if ?>


<?cs

if:Form["нст,дст"].0 || Form["прш,дст"].0
?>деепричастие
<?cs if:Form["нст,дст"].0
?>наст.:	<?cs call:out_variants(Form["нст,дст"]) ?>
<?cs /if ?><?cs if:Form["прш,дст"].0
?>прош.:	<?cs call:out_variants(Form["прш,дст"]) ?>
<?cs /if ?><?cs /if ?>
