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

// TODO: If nickname is missing, make a connecting & printing connection information first, after then get a nick name


/* Helper functions */
void PrintConnectingInfo(sockaddr_storage client_address, socklen_t socket_address_storage_size);
/* End of Helpers */


int main(int argc, char* argv[])
{
	// If input is incorrect,
	if (argc != 4)
	{
		std::cerr << "usage: " << argv[0] << " <server ip address> <port or service name> [nickname]\n";
		return 1;
	}

	const char* const server_host = argv[1];
	const char* const port = argv[2];
	const char* const nick_name = argv[3];

	// Get client socket
	const SocketLib::sock client_socket = SocketLib::OpenClientSocket(server_host, port);

	// If returned socket is invalid,
	if (client_socket == SocketLib::BAD_SOCKET)
	{
		std::cout << "Bad Socket!\n";
		return 1;
	}

	// '0' means I am writer
	const std::string identifierBuffer = "0";
	const int identifySent = send(client_socket, identifierBuffer.c_str(), identifierBuffer.length(), 0);

	// TODO: Print Connecting Information Here
	
	std::string input_line;
	while (true)
	{
		std::getline(std::cin, input_line);

		// if nothing inputted,
		if (input_line.empty() == true)
		{
			break;
		}

		// send input string to server
		// TODO: Prepend start and end delimiter
		const int bytes_sent = send(client_socket, input_line.c_str(), input_line.size(), 0);

		// if transferring is weird,
		if (bytes_sent <= 0)
		{
			std::cout << "errno : " << errno << std::endl;
			// print debug message and jump to next iteration
			std::cout << ((bytes_sent < 0) ? "Failed to send message\n" : "Nothing sent\n");
			break;
		}

		// sending is succeed
	}

	SocketLib::CloseSocket(client_socket);
	return 1;
}