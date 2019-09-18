PRG=cipher
PRGD=$(PRG)_d.exe
GCCFLAGS=-Wall -Wextra -pedantic -Wconversion -O2 -std=c++11 -Weffc++

SOURCE_FILES=cipher.cpp
DRIVER_FILES=driver.cpp

DR_MEMORY_OPTIONS=-batch -brief

UNITTESTS = 0 1 2 3 4 5 6 7 8
MEMTESTS = mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8

all : $(UNITTESTS) $(MEMTESTS)

$(PRG):
	g++ -o $(PRG) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -g
$(PRGD):
	g++ -o $(PRGD) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -O2

$(UNITTESTS) : $(PRG)
	@echo "running test$@"
	./$(PRG) $@ >studentout$@.txt
	diff out$@.txt studentout$@.txt $(DIFF_OPTIONS) > difference$@
$(MEMTESTS) : $(PRGD)
	@echo "running mem test $@"
	drmemory $(DR_MEMORY_OPTIONS) -- ./$(PRGD) $(subst mem,,$@) 1>/dev/null 2>difference$@.txt
clean:
	rm -f $(wildcard $(PRG) *.exe *.tds *.o *.obj *manifest* studentout* diff* results*)
