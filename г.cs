<?cs include:"header.cs"

?>== глагол ==

инф:	<?cs call:out_variants(Form["дст"]) ?>

<?cs

if:Form["ед,буд,1л,дст"].0 || Form["мн,буд,1л,дст"].0 || Form["ед,буд,2л,дст"].0
     || Form["мн,буд,2л,дст"].0 || Form["ед,буд,3л,дст"].0 || Form["мн,буд,3л,дст"].0
?>будущее время
1л:	<?cs call:out_variants(Form["ед,буд,1л,дст"]) ?>	<?cs call:out_variants(Form["мн,буд,1л,дст"]) ?>
2л:	<?cs call:out_variants(Form["ед,буд,2л,дст"]) ?>	<?cs call:out_variants(Form["мн,буд,2л,дст"]) ?>
3л:	<?cs call:out_variants(Form["ед,буд,3л,дст"]) ?>	<?cs call:out_variants(Form["мн,буд,3л,дст"]) ?>

<?cs /if
?><?cs

if:Form["ед,нст,1л,дст"].0 || Form["мн,нст,1л,дст"].0 || Form["ед,нст,2л,дст"].0
       || Form["мн,нст,2л,дст"].0 || Form["ед,нст,3л,дст"].0 || Form["мн,нст,3л,дст"].0
?>настоящее время
1л:	<?cs call:out_variants(Form["ед,нст,1л,дст"]) ?>	<?cs call:out_variants(Form["мн,нст,1л,дст"]) ?>
2л:	<?cs call:out_variants(Form["ед,нст,2л,дст"]) ?>	<?cs call:out_variants(Form["мн,нст,2л,дст"]) ?>
3л:	<?cs call:out_variants(Form["ед,нст,3л,дст"]) ?>	<?cs call:out_variants(Form["мн,нст,3л,дст"]) ?>

<?cs /if
?><?cs

if:Form["ед,мр,прш,дст"].0 || Form["мн,прш,дст"].0
       || Form["ед,жр,прш,дст"].0 || Form["ед,ср,прш,дст"].0
?>прошлое время
мр:	<?cs call:out_variants(Form["ед,мр,прш,дст"]) ?>	<?cs call:out_variants(Form["мн,прш,дст"]) ?>
жр:	<?cs call:out_variants(Form["ед,жр,прш,дст"]) ?>
ср:	<?cs call:out_variants(Form["ед,ср,прш,дст"]) ?>

<?cs /if
?><?cs

if:Form["ед,1л,пвл,дст"].0 || Form["мн,1л,пвл,дст"].0
       || Form["ед,2л,пвл,дст"].0 || Form["мн,2л,пвл,дст"].0
?>повелительное наклонение
<?cs if:Form["ед,1л,пвл,дст"].0 || Form["мн,1л,пвл,дст"].0
?>1л:	<?cs call:out_variants(Form["ед,1л,пвл,дст"]) ?>	<?cs call:out_variants(Form["мн,1л,пвл,дст"]) ?>
<?cs /if
?>2л:	<?cs call:out_variants(Form["ед,2л,пвл,дст"]) ?>	<?cs call:out_variants(Form["мн,2л,пвл,дст"]) ?>

<?cs /if
?><?cs

if Form["ед,им,мр,прш,од,но,дст"].0 || Form["ед,им,мр,нст,од,но,дст"].0
?>действительное причастие
<?cs if Form["ед,им,мр,нст,од,но,дст"].0 ?>наст.:	<?cs call:out_variants(Form["ед,им,мр,нст,од,но,дст"]) ?>
<?cs /if ?><?cs if Form["ед,им,мр,прш,од,но,дст"].0 ?>прош.:	<?cs call:out_variants(Form["ед,им,мр,прш,од,но,дст"]) ?>
<?cs /if ?>
<?cs /if ?><?cs

if:Form["ед,им,мр,нст,од,но,стр"].0 || Form["ед,им,мр,прш,од,но,стр"].0
?>страдательное причастие
<?cs if:Form["ед,им,мр,нст,од,но,стр"].0 ?>наст.:	<?cs call:out_variants(Form["ед,им,мр,нст,од,но,стр"]) ?>
<?cs /if
?><?cs if:Form["ед,им,мр,прш,од,но,стр"].0 ?>прош.:	<?cs call:out_variants(Form["ед,им,мр,прш,од,но,стр"]) ?>
<?cs /if ?>
<?cs /if ?><?cs

if:Form["нст,дст"].0 || Form["прш,дст"].0
?>деепричастие
<?cs if:Form["нст,дст"].0
?>наст.:	<?cs call:out_variants(Form["нст,дст"]) ?>
<?cs /if ?><?cs if:Form["прш,дст"].0
?>прош.:	<?cs call:out_variants(Form["прш,дст"]) ?>
<?cs /if ?><?cs /if ?>
