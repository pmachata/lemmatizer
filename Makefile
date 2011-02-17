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
# <http://www.gnu.org/licenses/>.

SEMAN_INCLUDE = /usr/include/Seman
TARGETS = lemmatizer
CXXPPFLAGS += $(CXXINCLUDES) -I$(SEMAN_INCLUDE)
CXXFLAGS = -g -O2
override CXXFLAGS += -Wall -Wno-unknown-pragmas $(CXXPPFLAGS)
LDFLAGS =

DIRS = .
ALLSOURCES = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cc $(dir)/*.hh $(dir)/*.ii)) Makefile
CCSOURCES = $(filter %.cc,$(ALLSOURCES))
DEPFILES = $(patsubst %.cc,%.cc-dep,$(CCSOURCES))

all: $(TARGETS)

lemmatizer: override LDFLAGS += -lLemmatizerrsh -lAgramtabrsh -lGraphanrsh 	\
	-lStructDictrsh -lMorphWizardrsh -lneo_cs -lneo_utl -lfcgi \
	-lboost_filesystem -lboost_system -lboost_program_options
hdf.o main.o template_cache.o: CXXPPFLAGS += -I/usr/include/ClearSilver
lemmatizer: adjective.o backend.o default.o fcgi_backend.o format.o	\
	forms.o gramcodes.o hdf.o lemmatize.o main.o noun.o number.o	\
	part_of_speech.o pos_handler.o rus_gender.o rus_gramtab.o	\
	simple.o template_cache.o url_decode.o verb.o

-include $(DEPFILES)

%.cc-dep: %.cc
	$(CXX) $(CXXFLAGS) -MM -MT '$(<:%.cc=%.o) $@' $< > $@
$(TARGETS):
	$(CXX) $^ -o $@ $(LDFLAGS)

test-%: %.o %.cc test.o
	$(CXX) $(CXXFLAGS) -DSELFTEST $(@:test-%=%.cc) $(filter-out $<,$(filter %.o,$^)) -o $@ $(LDFLAGS)
	./$@ || (rm -f $@; exit 1)

clean:
	rm -f $(foreach dir,$(DIRS),$(dir)/*.o $(dir)/*.*-dep) $(TARGETS)

.PHONY: all clean dist
