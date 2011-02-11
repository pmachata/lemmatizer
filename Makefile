SEMAN_INCLUDE = /usr/include/Seman
TARGETS = lemmatizer
CXXPPFLAGS = $(CXXINCLUDES) -I$(SEMAN_INCLUDE)
CXXFLAGS = -Wall -Wno-unknown-pragmas -g -O2 $(CXXPPFLAGS)
LDFLAGS =

DIRS = .
ALLSOURCES = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cc $(dir)/*.hh $(dir)/*.ii)) Makefile
CCSOURCES = $(filter %.cc,$(ALLSOURCES))
DEPFILES = $(patsubst %.cc,%.cc-dep,$(CCSOURCES))

all: $(TARGETS)

lemmatizer: LDFLAGS += -lLemmatizerrsh -lAgramtabrsh -lGraphanrsh 	\
	-lStructDictrsh -lMorphWizardrsh -lneo_cs -lneo_utl -lfcgi \
	-lboost_filesystem -lboost_system
template_cache.o main.o: CXXPPFLAGS += -I/usr/include/ClearSilver
lemmatizer: adjective.o backend.o fcgi_backend.o format.o forms.o	\
	gramcodes.o lemmatize.o main.o noun.o part_of_speech.o		\
	pos_handler.o rus_gender.o rus_gramtab.o simple.o		\
	template_cache.o url_decode.o verb.o

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
