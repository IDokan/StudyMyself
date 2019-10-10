PRG=lazycopy
GCC=g++
GCCFLAGS=-Werror -Wall -Wextra -std=c++14 -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder -Werror=vla

VALGRIND_OPTIONS=--leak-check=full --error-exitcode=1 --vgdb=no --track-fds=yes
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines

SOURCE_FILES=array.cpp element1.cpp element2.cpp
DRIVER_FILES=driver.cpp element-factory.cpp

test: | release 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
valgrind: | debug mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16
debug:
	$(GCC) -o $(PRG) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -g
release:
	$(GCC) -o $(PRG) $(DRIVER_FILES) $(SOURCE_FILES) $(GCCFLAGS) -O2
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16:
	echo "running test$@"
	python watchdog.py 200 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFF_OPTIONS) > difference$@
mem0 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 mem9 mem10 mem11 mem12 mem13 mem14 mem15 mem16:
	echo "running memory test $@"
	python watchdog.py 2000 valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>results$@ 2>&1
clean:
	rm -f $(wildcard $(PRG) *.exe *.o *.obj *.tds *.exe.manifest student* difference* results* *.pyc)
