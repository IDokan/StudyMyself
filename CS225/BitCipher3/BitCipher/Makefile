PRG=cipher
PRGD=$(PRG)_d.exe
GCCFLAGS=-pedantic -O2 -std=c++11

SOURCE_FILES=cipher.cpp
DRIVER_FILES=driver.cpp

DR_MEMORY_OPTIONS=-batch -brief

UNITTESTS = 0 1 2 3 4 5 6 7 8 9
MEMTESTS = mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8

all : $(UNITTESTS)

$(PRG): cipher.cpp driver.cpp
	g++ -o $(PRG) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -g
$(PRGD): cipher.cpp driver.cpp
	g++ -o $(PRGD) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -O2

$(UNITTESTS) : $(PRG)
	@echo "running test$@"
	$(PRG) $@ > studentout$@.txt
	diff out$@.txt studentout$@.txt > difference$@
$(MEMTESTS) : $(PRGD)
	@echo "running mem test $@"
	drmemory $(DR_MEMORY_OPTIONS) -- ./$(PRGD) $(subst mem,,$@) 1>/dev/null 2>difference$@.txt
clean:
	rm -f $(wildcard $(PRG) *.exe *.tds *.o *.obj *manifest* studentout* diff* results*)
