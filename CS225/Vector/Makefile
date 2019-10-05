PRG=final.exe
PRGD=final_d.exe
GCCFLAGS=-Wall -Wextra -pedantic -Wconversion -O2 -std=c++11 -Weffc++

OBJECTS0=
DRIVER0=Driver.cpp
HEADERS=Vector.h Vector.hpp

UNITTESTS = 1 2 3 4 5 6
MEMTESTS = mem1 mem2 mem3 mem4 mem5 mem6

DR_MEMORY_OPTIONS=-batch -brief
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines -b

OSTYPE := $(shell uname)
ifeq (,$(findstring CYGWIN,$(OSTYPE)))
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

all : $(UNITTESTS) $(MEMTESTS)
unittests : $(UNITTESTS)
memtests : $(MEMTESTS)
$(PRG): $(OBJECTS0) $(DRIVER0) $(HEADERS)
	g++ -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(CYGWIN) -O2
$(PRGD): $(OBJECTS0) $(DRIVER0) $(HEADERS)
	g++ -o $(PRGD) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS) $(CYGWIN) -g

$(UNITTESTS) : $(PRG)
	@echo "should run in less than 200 ms"
	./$(PRG) $@ >studentout$@.txt
	diff out$@ studentout$@.txt $(DIFF_OPTIONS) > difference$@
$(MEMTESTS): $(PRGD)
	@echo "running mem test $@"
	drmemory $(DR_MEMORY_OPTIONS) -- ./$(PRGD) $(subst mem,,$@) 1>/dev/null 2>difference$@.txt
clean : 
	rm *.exe student* difference*
