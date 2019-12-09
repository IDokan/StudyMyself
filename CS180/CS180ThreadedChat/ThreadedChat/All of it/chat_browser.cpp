// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: chat_browser.cpp
// Purpose:   To implement the writer program.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Sinil gang <sinil.gang@digipen.edu>, Jeesoo Kim <seanseanseankim@gmail.com>
// Creation date: 2018/11/30

#include <iostream>
#include <array>
#include <string>
#include <signal.h>
#include "SocketLib.h"

bool should_run = true;

int main(int argc, char* argv[])
{
	// If input is incorrect,
	if (argc != 3)
	{
		std::cerr << "usage: " << argv[0] << " <server ip address> <port or service name>\n";
		return 1;
	}


	const char* const server_host = argv[1]; //ip address where server is opened
	const char* const port = argv[2]; //port number 

	//Get a client socket
	const SocketLib::sock browser_socket = SocketLib::OpenClientSocket(server_host, port);

	//If an error occured while getting the socket
	if (browser_socket == SocketLib::BAD_SOCKET)
	{
		std::cout << "Bad Socket!\n";
		return 1;
	}

	// Send message Who I am
	const std::string identifierBuffer = "browser";
	SocketLib::SendString(browser_socket, identifierBuffer);

	std::string message_from_server;
	while (should_run)
	{

		if (bool is_recv_success = SocketLib::GetInputWithBuffer(browser_socket, message_from_server);
			is_recv_success == true)
		{
			std::cout << message_from_server << std::endl;
		}
		else
		{
			should_run = false;
		}
	} //If there are no more messages to receive
	SocketLib::CloseSocket(browser_socket);
	return 0;
}
