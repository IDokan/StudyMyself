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

int should_run = true;

void Quit(int /*sig*/)
{
	should_run = false;
	//should I just exit(EXIT_SUCCESS) here?
}

int main(int argc, char* argv[])
{
	// If input is incorrect,
	if (argc != 3)
	{
		std::cerr << "usage: " << argv[0] << " <server ip address> <port or service name>\n";
		return 1;
	}

	signal(SIGINT, Quit);
	
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
	
	std::array<char, 1024> receive_buffer{};
	while(should_run)
	{
		
		/************************************************************************************************************
		    TODO : - Print the browser number (std::cout << browser_num.last().id++)
		           - Print the currently joined writers (std::cout << active_writers.first() ~ active_writers.last())
		           - Print history chats if any exist (std::cout << chat_history.first() ~ chat_history.last()) 
		 ************************************************************************************************************/

		/*Once a browser is connected to the server, it loads the history */
		int bytes_received = 0;
		int current_bytes_received;
		do
		{
			//receive from the server
			//the recv() function returns the length of the message written to the receive_buffer
			current_bytes_received = recv(browser_socket, receive_buffer.data(), receive_buffer.size(), 0);
			/***********************************************************************************************************
					Trying to implement a custom "recv", with enum struct messageType
					enum MessageType{
					Chat,
					Debug,
					Ping,
					ClientType
					}			  
			 ************************************************************************************************************/
			//add the length of the message to the bytes_received variable
			bytes_received += current_bytes_received;

			//if at least more than character has been written 
			if(current_bytes_received > 0)
			{
			
				receive_buffer[current_bytes_received] = 0;
				//Identify who sent the message 
				//std::cout << get_nickname << "> ";
				std::cout << receive_buffer.data();
				
			}
		} while (current_bytes_received != 0); //If there are no more messages to receive
		std::cout << "\n";
		
		/************************************************************************************************************
		    TODO : If a new person joins the chat, we need to print "<new_nickname> has joined the chat room!" 
			Maybe just receive from the server? Browser just receives it as the message I think,,
		 ************************************************************************************************************/
		
		SocketLib::CloseSocket(browser_socket);
		return 0;
	}
}