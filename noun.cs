<?cs include:"header.cs" ?>== существительное ==
род:	<?cs call:out_variants(Form.gender) ?>

	ед	мн
им.	<?cs call:out_variants(Form.singular.nominative) ?>	<?cs call:out_variants(Form.plural.nominative) ?>
р.	<?cs call:out_variants(Form.singular.genitive) ?>	<?cs call:out_variants(Form.plural.genitive) ?>
д.	<?cs call:out_variants(Form.singular.dative) ?>	<?cs call:out_variants(Form.plural.dative) ?>
в.	<?cs call:out_variants(Form.singular.accusative) ?>	<?cs call:out_variants(Form.plural.accusative) ?>
тв.	<?cs call:out_variants(Form.singular.instrumental) ?>	<?cs call:out_variants(Form.plural.instrumental) ?>
пр.	<?cs call:out_variants(Form.singular.prepositional) ?>	<?cs call:out_variants(Form.plural.prepositional) ?>
