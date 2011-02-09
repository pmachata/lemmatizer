SEMAN_INCLUDE = /usr/include/Seman

all: TestLem
TestLem: main.o gramcodes.o forms.o lemmatize.o part_of_speech.o \
	pos_handler.o noun.o adjective.o format.o rus_gramtab.o simple.o
	g++ $^ -o $@ -g $(LDFLAGS) \
		-lLemmatizerrsh -lAgramtabrsh -lGraphanrsh -lStructDictrsh -lMorphWizardrsh \
		-lneo_cs -lneo_utl

%.o: %.cc
	g++ $(CXXFLAGS) -I$(SEMAN_INCLUDE) -g $< -Wall -Wno-unknown-pragmas -c -o $@

main.o: CXXFLAGS += -I/usr/include/ClearSilver

clean:
	rm -f *.o TestLem
