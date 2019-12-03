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
#include <thread>
#include <mutex>
#include <string>
#include <algorithm>
#include "SocketLib.h"

struct Message
{
	std::string nickname;
	std::string string;
};

namespace
{
	constexpr char WRITER = '0';
	constexpr char BROWSER = '1';
	constexpr int bufferSize = 512;

	/* TODO: Make sure these things. Currently, Not Sure all of it */
	// The chat server must keep track of all connected writer clients names.
	// I guess it will be used for display Writers: <Who>, <Who>
	namespace NicknameData
	{
		std::vector<std::string> connected_writers_nickname;
		std::mutex mtx;
	};
	// It should store a set of browser clients separately.
	namespace BrowserData
	{
		std::vector<SocketLib::sock> connected_browsers;
		std::mutex mtx;
	};
	// It should maintain a history of all messages received from the writer clients.
	namespace MessageData
	{
		std::vector<Message> history;
		std::mutex mtx;
	};
}
// TODO: Static struc? or idividual variables
/*		For example,

	std::vector<std::string> connected_writers_nickname;
	std::mutex nickname_mutex;

	...

	std::vector<Message> history;
	std::mutex history_mutex;
*/


/* Helper functions */
void DoWriterThing(const SocketLib::sock client_socket);
void DoBrowserThing(SocketLib::sock client_socket);
Message Pack(std::string nickname, std::string chatting);
void SendChatroomProperty(SocketLib::sock client_socket);
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
	const SocketLib::sock listen_socket = OpenListener(port, SocketLib::AddressFamily::IPv4);
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
		const SocketLib::sock new_client_data_socket = accept(listen_socket, (sockaddr*)& client_address, &socket_address_storage_size);

		if (new_client_data_socket == SocketLib::BAD_SOCKET)
		{
			continue;
		}

		// Print connection information
		SocketLib::PrintConnectToClient(client_address, socket_address_storage_size);

		// identify if it is a writer or browser client.
		std::string identifier = SocketLib::GetInputWithBuffer(new_client_data_socket);

		// TODO: DEBUG PRINT
		std::cout << "Identifier : " << identifier << std::endl;
		
		if (identifier.at(0) == WRITER)
		{
			// Send the number of Browsers and Writers nickname
			/*
			Browsers: %d
			 Writers:
			*/
			//TODO: Where get client's nickname?

			// Start thread!
			std::thread writerThread{ DoWriterThing, new_client_data_socket };
			writerThread.detach();
		}
		else if (identifier.at(0) == BROWSER)
		{
			DoBrowserThing(new_client_data_socket);
		}


		// TODO: There should be a server generated message each time a new writer client joins or exits.
		//When a new writer joins the chat room
		//if()  
	}
}

void DoWriterThing(const SocketLib::sock client_socket)
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
	std::string nickname = SocketLib::GetInputWithBuffer(client_socket);
	{std::scoped_lock lock(NicknameData::mtx);
	NicknameData::connected_writers_nickname.push_back(nickname);

	//// TODO: DEBUG OUT, SHOULD REMOVED
	//std::for_each(begin(NicknameData::connected_writers_nickname), end(NicknameData::connected_writers_nickname), [](const std::string& nickname)
	//	{
	//		std::cout << nickname << std::endl;
	//	});
	//std::cout << std::endl;
	}
	
	SendChatroomProperty(client_socket);
	
	while (true)
	{
		std::string inputMessage = SocketLib::GetInputWithBuffer(client_socket);

		// TODO: DEBUG OUT, SHOULD REMOVED
		std::cout << inputMessage << std::endl;

		{	std::scoped_lock lock(MessageData::mtx);
		MessageData::history.push_back(Pack(nickname, inputMessage));

			std::scoped_lock browser_lock(BrowserData::mtx);
			for (const auto& socket : BrowserData::connected_browsers)
			{
				SocketLib::SendString(socket, inputMessage);
			}
		}
	}
}

void DoBrowserThing(SocketLib::sock client_socket)
{
	// Do browser thing
	/*	0. Current Status output
	 *	1. Send all message history when connected,
	 *	2. Add it to the vector that named connected_browser
	 *
	 */

	SendChatroomProperty(client_socket);


	// TODO: I'm not sure history should be locked.....
	std::scoped_lock lock(MessageData::mtx);
	for (auto& msg : MessageData::history)
	{
		SocketLib::SendString(client_socket, msg.string);
	}
	std::scoped_lock browserLock(BrowserData::mtx);
	BrowserData::connected_browsers.push_back(client_socket);
}

Message Pack(std::string nickname, std::string chatting)
{
	return Message{ nickname.c_str(), chatting.c_str() };
}

void SendChatroomProperty(SocketLib::sock client_socket)
{
	SocketLib::SendString(client_socket, std::to_string(BrowserData::connected_browsers.size()));

	{	std::scoped_lock lock(NicknameData::mtx);
		for (const auto& nickname : NicknameData::connected_writers_nickname)
		{
			SocketLib::SendString(client_socket, nickname);
		}
	}
	SocketLib::SendString(client_socket, SocketLib::TERMINATE_CHAR);
}
