<?cs

def:out_variants(val) ?><?cs
  each:item = val ?><?cs
    if !first(item) ?>,<?cs /if ?><?cs
    var:item ?><?cs
  /each ?><?cs
/def ?><?cs

def:html_variants(val) ?><?cs
  each:item = val ?><?cs
    if !first(item) ?><br/><?cs /if ?><?cs
    var:item ?><?cs
  /each ?><?cs
/def ?>
