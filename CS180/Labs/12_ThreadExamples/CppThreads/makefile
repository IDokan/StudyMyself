CC = g++
CFLAGS = -Og -Wall -Wextra -std=c++17

all: CppThread1 CppThread2 CppThreadMulti1 CppThreadMulti2 CppThreadData CppThreadMutex

CppThread1:
	$(CC) $(CFLAGS) -o CppThread1 CppThread1.cpp -lpthread

CppThread2:
	$(CC) $(CFLAGS) -o CppThread2 CppThread2.cpp -lpthread

CppThreadMulti1:
	$(CC) $(CFLAGS) -o CppThreadMulti1 CppThreadMulti1.cpp -lpthread

CppThreadMulti2:
	$(CC) $(CFLAGS) -o CppThreadMulti2 CppThreadMulti2.cpp -lpthread

CppThreadData:
	$(CC) $(CFLAGS) -o CppThreadData CppThreadData.cpp -lpthread

CppThreadMutex:
	$(CC) $(CFLAGS) -o CppThreadMutex CppThreadMutex.cpp -lpthread

clean:
	rm -rf $(wildcard *.o) CppThread1 CppThread2 CppThreadMulti1 CppThreadMulti2 CppThreadData CppThreadMutex