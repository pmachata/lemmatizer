SEMAN_INCLUDE = $(SEMAN_DIR)/Source
SEMAN_LIB = $(SEMAN_DIR)/Bin

all: TestLem
TestLem: main.o gramcodes.o forms.o lemmatize.o
	g++ $^ -o $@ -g -L$(SEMAN_LIB) -Wl,-rpath,$(SEMAN_LIB) \
		-lLemmatizerrsh -lAgramtabrsh -lGraphanrsh -lStructDictrsh -lMorphWizardrsh \
		-lpcre -lpcrecpp -lctemplate

%.o: %.cc
	g++ -I$(SEMAN_INCLUDE) -g $< -Wall -Wno-unknown-pragmas -c -o $@

clean:
	rm -f *.o TestLem
