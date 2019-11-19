CC = g++
CFLAGS = -O2 -Wall -Wextra -std=c++17

all: echo_client echo_server

echo_client: echo_client.o sockets.o
	$(CC) $(CFLAGS) -o echo_client echo_client.o sockets.o

echo_server: echo_server.o sockets.o
	$(CC) $(CFLAGS) -o echo_server echo_server.o sockets.o

echo_client.o: echo_client.cpp
	$(CC) $(CFLAGS) -c echo_client.cpp

echo_server.o: echo_server.cpp
	$(CC) $(CFLAGS) -c echo_server.cpp

sockets.o: sockets.cpp
	$(CC) $(CFLAGS) -c sockets.cpp

clean:
	rm -rf $(wildcard *.o) echo_client echo_server