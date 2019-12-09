CC = g++
CFLAGS = -O2 -Wall -Wextra -std=c++17 -pthread

all: chat_browser chat_writer chat_server

chat_browser: chat_browser.o SocketLib.o
	$(CC) $(CFLAGS) -o chat_browser chat_browser.o SocketLib.o 

chat_writer: chat_writer.o SocketLib.o
	$(CC) $(CFLAGS) -o chat_writer chat_writer.o SocketLib.o

chat_server: chat_server.o SocketLib.o
	$(CC) $(CFLAGS) -o chat_server chat_server.o SocketLib.o

chat_browser.o: chat_browser.cpp
	$(CC) $(CFLAGS) -c chat_browser.cpp

chat_writer.o: chat_writer.cpp
	$(CC) $(CFLAGS) -c chat_writer.cpp

chat_server.o: chat_server.cpp
	$(CC) $(CFLAGS) -c chat_server.cpp	

SocketLib.o: SocketLib.cpp
	$(CC) $(CFLAGS) -c SocketLib.cpp

clean:
	rm -rf $(wildcard *.o) chat_browser chat_writer chat_server