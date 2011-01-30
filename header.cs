<?cs def:out_variants(val) ?><?cs
  each:item = val ?><?cs
    if !first(item) ?>,<?cs /if ?><?cs
    var:item ?><?cs
  /each ?><?cs
/def

?>
