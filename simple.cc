#include "simple.hh"
#include "forms.hh"
#include "format.hh"
#include "part_of_speech.hh"

simple_handler::simple_handler ()
  : pos_handler ("simple")
{
}

void
simple_handler::fill_hdf (CAgramtab *agramtab,
			 lemmatize::const_iterator const &it,
			 hdf_data_map &data) const
{
  lemmatize::forms forms = it.forms ();
  part_of_speech pos = it.get_part_of_speech ();
  data["pos"].push_back (std::make_pair (format_rus (pos.number_as<pos_code_t> ()), -1));

  for (lemmatize::forms::const_iterator ft = forms.begin ();
       ft != forms.end (); ++ft)
    data["word"].push_back (std::make_pair (*ft, ft.accent ()));
}
