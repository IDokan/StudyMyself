// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: chat_writer.cpp
// Purpose:   To implement the writer program.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Sinil gang <sinil.gang@digipen.edu>, Jeesoo Kim <seanseanseankim@gmail.com>
// Creation date: 2018/11/30


#include <iostream>
#include <array>
#include <string>
#include "SocketLib.h"
/*Global variables*/
int should_run = true;

// TODO: If nickname is missing, make a connecting & printing connection information first, after then get a nick name
int main(int argc, char* argv[])
{
	// If input is incorrect,
	if (argc < 3 || argc > 4)
	{
		std::cerr << "usage: " << argv[0] << " <server ip address> <port or service name> [nickname]\n";
		return 1;
	}

	const char* const server_host = argv[1];
	const char* const port = argv[2];

	// Get client socket
	const SocketLib::sock client_socket = SocketLib::OpenClientSocket(server_host, port);

	// If returned socket is invalid,
	if (client_socket == SocketLib::BAD_SOCKET)
	{
		std::cout << "Bad Socket!\n";
		return 1;
	}

	// "writer" means I am writer
	const std::string identifierBuffer = "writer";
	SocketLib::SendString(client_socket, identifierBuffer);

	std::string nick_name;
	if (argc == 4)
	{
		nick_name = argv[3];
	}
	else
	{
		std::cout << "Please enter in your nickname : ";
		std::getline(std::cin, nick_name);
	}
	// Get a chat room information
	std::cout << "\nDEBUG : Try to get a chat room information\n";	// DEBUG
	std::string chat_room_info;
	SocketLib::GetInputWithBuffer(client_socket, chat_room_info);
	std::cout << chat_room_info << std::endl;
	std::cout << "\nDEBUG : Success to get a chat room information\n";	// DEBUG

	// Send string later
	SocketLib::SendString(client_socket, nick_name);

	std::string input_line;
	while (should_run)
	{
		std::getline(std::cin, input_line);

		// if nothing inputted,
		if (input_line.empty() == true)
		{
			break;
		}

		SocketLib::SendString(client_socket, input_line);

		// sending is succeed
	}

	SocketLib::CloseSocket(client_socket);
	return 1;
}