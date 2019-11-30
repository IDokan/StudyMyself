// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: chat_server.cpp
// Purpose:   To implement the server.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Sinil gang <sinil.gang@digipen.edu>, Jeesoo Kim <seanseanseankim@gmail.com>
// Creation date: 2018/11/30

#include <iostream>
#include <vector>
#include <array>
#include "SocketLib.h"

struct Message
{
	const char* nickname;
	const char* string;
};

namespace
{
	constexpr char WRITER = '0';
	constexpr char BROWSER = '1';
	constexpr int bufferSize = 512;
	
	/* TODO: Make sure these things. Currently, Not Sure all of it */
	// The chat server must keep track of all connected writer clients names.
	// I guess it will be used for display Writers: <Who>, <Who>
	std::vector<std::string> connected_writers_nickname;
	// It should store a set of browser clients separately.
	std::vector<SocketLib::sock> connected_browsers;
	// It should maintain a history of all messages received from the writer clients.
	std::vector<Message> history;
}


/* Helper functions */
void PrintConnectingInfo(sockaddr_storage client_address, socklen_t socket_address_storage_size);
void DoWriterThing();
void DoBrowserThing();
/* End of Helpers */
int main(int argc, char* argv[])
{
	// If input is incorrect,
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " <port or service name>\n";
		return 1;
	}

	const char* port = argv[1];
	const SocketLib::sock listen_socket = SocketLib::OpenListener(port, SocketLib::AddressFamily::IPv4);
	socklen_t socket_address_storage_size = sizeof(sockaddr_storage);
	bool should_run = true;

	// There will be one thread that is spinning on a while() loop,
	// waiting for clients to attach to the socket.
	// When it detects a writer client,
	// it will create a writer client thread.

		// looping until should_run become false
	while (should_run)
	{
		sockaddr_storage client_address = {};
		// Call accept() to wait for connection request and to get a new connection socket to communicate with
		const SocketLib::sock new_client_data_socket = accept(listen_socket, (sockaddr*)&client_address, &socket_address_storage_size);

		if (new_client_data_socket == SocketLib::BAD_SOCKET)
		{
			continue;
		}

		// Print connection information
		PrintConnectingInfo(client_address, socket_address_storage_size);
		
		// TODO: identify if it is a writer or browser client.
		std::array<char, bufferSize> identifyingBuffer{};
		long long bytes_received;
		do
		{
			bytes_received = recv(new_client_data_socket, &identifyingBuffer.front(), bufferSize, 0);
		}
		// TODO: Check it is work , done!
		while (bytes_received <= 0);
		if (identifyingBuffer.at(0) == WRITER)
		{
			DoWriterThing();
		}
		else if (identifyingBuffer.at(0) == BROWSER)
		{
			DoBrowserThing();
		}


		  // TODO: There should be a server generated message each time a new writer client joins or exits.
	}
}


// TODO: They will be printed all in each platform
void PrintConnectingInfo(sockaddr_storage client_address, socklen_t socket_address_storage_size)
{
	constexpr int NameBufferLength = 512;
	std::array<char, NameBufferLength> client_hostname{};
	std::array<char, NameBufferLength> client_port{};
	const auto psocketaddress_information = reinterpret_cast<sockaddr*>(&client_address);
	// Call getnameinfo() to get a string version of the clients (IP:Port) information
	getnameinfo(psocketaddress_information, socket_address_storage_size, &client_hostname.front(), NameBufferLength, &client_port.front(), NameBufferLength, NI_NUMERICHOST);
	std::cout << "Connected to (" << client_hostname.data() << ", " << client_port.data() << ") / ";
	getnameinfo(psocketaddress_information, socket_address_storage_size, &client_hostname.front(), NameBufferLength, &client_port.front(), NameBufferLength, NI_NUMERICSERV);
	std::cout << '(' << client_hostname.data() << ", " << client_port.data() << ")\n\n";
}

void DoWriterThing()
{
	// It should create a thread for each writer client, so that it is constantly receiving messages.
	// Does it mean writer -> Peer Thread, browser -> Main Thread
	// Do writer thing
	/*
	 * There will be one thread for each writer client.
	 * That thread will typically be blocked reading from the socket.
	 * When it receives a line of text from the socket,
	 * it will construct the proper string from it,
	 * put that string onto the chat room's list,
	 * and signal the chat room server.
	 *
	 *
	 * TODO: Messages should be prepended with the source writer clients name.
	 */

	while (true)
	{
		std::cout << "Writer connected!\n";
	}
}

void DoBrowserThing()
{
	// Do browser thing
	/*
	 *
	 *
	 */
	while (true)
	 {
		 std::cout << "Browser connected!\n";
	 }
}

