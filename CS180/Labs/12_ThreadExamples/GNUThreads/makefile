CC = g++
CFLAGS = -Og -Wall -Wextra -std=c++17

all: GNUThread1 GNUThread2 GNUThreadMulti1 GNUThreadMulti2 GNUThreadData GNUThreadMutex

GNUThread1:
	$(CC) $(CFLAGS) -o GNUThread1 GNUThread1.cpp -lpthread

GNUThread2:
	$(CC) $(CFLAGS) -o GNUThread2 GNUThread2.cpp -lpthread

GNUThreadMulti1:
	$(CC) $(CFLAGS) -o GNUThreadMulti1 GNUThreadMulti1.cpp -lpthread

GNUThreadMulti2:
	$(CC) $(CFLAGS) -o GNUThreadMulti2 GNUThreadMulti2.cpp -lpthread

GNUThreadData:
	$(CC) $(CFLAGS) -o GNUThreadData GNUThreadData.cpp -lpthread

GNUThreadMutex:
	$(CC) $(CFLAGS) -o GNUThreadMutex GNUThreadMutex.cpp -lpthread

clean:
	rm -rf $(wildcard *.o) GNUThread1 GNUThread2 GNUThreadMulti1 GNUThreadMulti2 GNUThreadData GNUThreadMutex