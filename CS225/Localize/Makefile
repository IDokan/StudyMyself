PREPROCESSPRG = Preprocess.exe
PRG=Localize.exe
PRGD=$(PRG)_d.exe
GCCFLAGS=-Wall -Wextra -pedantic -Wconversion -O2 -std=c++11 -Weffc++
DR_MEMORY_OPTIONS=-batch -brief

PREPROCESS_FILES=Preprocess.cpp
SOURCE_FILES=LocalizeText.cpp
DRIVER_FILES=driver.cpp

UNITTESTS = 0 1 2 3 4 5 6 7 8 9
MEMTESTS = mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9

all : $(UNITTESTS) $(MEMTESTS)

$(PREPROCESSPRG) : $(PREPROCESS_FILES) English.txt French.txt Italian.txt Spanish.txt Korean.txt
	g++ -o $(PREPROCESSPRG) $(PREPROCESS_FILES) -O2
	./$(PREPROCESSPRG)
$(PRG): $(SOURCE_FILES) $(DRIVER_FILES)
	g++ -o $(PRG) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -O2
$(PRGD): $(SOURCE_FILES) $(DRIVER_FILES)
	g++ -o $(PRGD) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -g

$(UNITTESTS) : $(PREPROCESSPRG) $(PRG)
	@echo "running test$@"
	./$(PRG) $@ > studentout$@.txt
	diff out$@.txt studentout$@.txt > difference$@
$(MEMTESTS) : $(PREPROCESSPRG) $(PRGD)
	@echo "running mem test $@"
	drmemory $(DR_MEMORY_OPTIONS) -- ./$(PRGD) $(subst mem,,$@) 1>/dev/null 2>difference$@.txt
clean:
	rm -f $(wildcard $(PRG) *.exe *.tds *.o *.obj *manifest* studentout* diff* results* *.dat strings.h) 
