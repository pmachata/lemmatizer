SEMAN_INCLUDE = $(SEMAN_DIR)/Source
SEMAN_LIB = $(SEMAN_DIR)/Bin

all: TestLem
TestLem: main.o gramcodes.o forms.o lemmatize.o part_of_speech.o
	g++ $^ -o $@ -g $(LDFLAGS) -L$(SEMAN_LIB) -Wl,-rpath,$(SEMAN_LIB) \
		-lLemmatizerrsh -lAgramtabrsh -lGraphanrsh -lStructDictrsh -lMorphWizardrsh \
		-lpcre -lpcrecpp -lneo_cs -lneo_utl

%.o: %.cc
	g++ $(CXXFLAGS) -I$(SEMAN_INCLUDE) -g $< -Wall -Wno-unknown-pragmas -c -o $@

main.o: CXXFLAGS += -I/usr/include/ClearSilver

clean:
	rm -f *.o TestLem
