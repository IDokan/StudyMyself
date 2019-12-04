// Copyright (C) 2018 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: sockets.cpp
// Purpose:   To implement the platform specifics needed for berkeley sockets.
// Language:  C++
// Platform:  Visual Studio 15.8.2 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Rudy Castan <rudy.castan@digipen.edu>
// Creation date: 2018/11/08

#include <array>
#include <iostream>
#include "SocketLib.h"

#ifndef AI_ADDRCONFIG
#define AI_ADDRCONFIG 0x400
#endif

namespace SocketLib
{
	// ========================================================================
	//  This class is designed to be a global singleton that initializes
	//  and shuts down Winsock.
	// ========================================================================
#ifdef _WIN32
	class System
	{
	public:
		System()
		{
			// attempt to start up the winsock lib
			WSADATA windowsSocketData = {};
			// The current version of the Windows Sockets specification is version 2.2.
			WSAStartup(MAKEWORD(2, 2), &windowsSocketData);
			//std::clog << windowsSocketData.szDescription << '\n';
			//std::clog << windowsSocketData.szSystemStatus << "\n\n\n";
		}
		~System()
		{
			// attempt to close down the winsock lib
			WSACleanup();
		}
	};
	System g_system;
#endif
}

namespace
{
	/**
	 * \brief Calls getaddrinfo and automatically prints gai_strerror to stderror when there is a problem. getaddrinfo()
	 * converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked
	 * list of struct addrinfo structures. \param host can be either a domain name, such as "example.com", an address
	 * string, such as "127.0.0.1", or NULL, in which case the address 0.0.0.0 or 127.0.0.1 is assigned depending on the
	 * hints flags. \param service can be a port number passed as string, such as "80", or a service name, e.g. "echo".
	 * \param hints can be either nullptr or an addrinfo structure with the type of service requested.
	 * \param results is a pointer that points to a new addrinfo structure with the information requested after
	 * successful completion of the function. \return The function returns true upon success and false if it fails.
	 */
	bool CanGetAddressInfo(const char* host, const char* service, const addrinfo* hints, addrinfo** results);
}

namespace SocketLib
{
	sock OpenListener(const char* service, AddressFamily address_family)
	{
		// The variable that contain the value of hints?
		// Specification of how to get a result in requested format (default setting)
		addrinfo hints{};

		// Using TCP, SOCK_STREAM is a identfier that indicates we are going to use TCP stream
		hints.ai_socktype = SOCK_STREAM;
		// Make return socket address that will be capable of accepting connections.
		// #ifdef AI_PASSIVE -> using bind(), listen(), accept(), if not using connect()
		hints.ai_flags = AI_PASSIVE;

		// Set address setting with given address family data
		switch (address_family)
		{
		case AddressFamily::IPv4:
			hints.ai_family = AF_INET;
			break;
		case AddressFamily::IPv6:
			hints.ai_family = AF_INET6;
			break;
		case AddressFamily::HostPreferred:
			hints.ai_flags |= AI_ADDRCONFIG;
			break;
		default:
			break;
		}

		addrinfo* addressResult = nullptr;

		if (const auto foundInformation = ::CanGetAddressInfo(nullptr, service, &hints, &addressResult);
			foundInformation == false)
		{
			// If failed, return BAD_SOCKET;
			return BAD_SOCKET;
		}
		sock listenSocket{ BAD_SOCKET };
		constexpr int SUCCESS = 0;
		for (addrinfo* currentAddress = addressResult; currentAddress != nullptr; currentAddress = currentAddress->ai_next)
		{
			/*socket() : Creates an endpoint for communication and returns a socket descriptor representing the endpoint.
			Different types of sockets provide different communication services.*/
			listenSocket = socket(currentAddress->ai_family, currentAddress->ai_socktype, currentAddress->ai_protocol);

			if (listenSocket == BAD_SOCKET)
				continue; //Socket failed, try the next one

			/*bind() : Associates a local address and a socket*/
			if (bind(listenSocket, currentAddress->ai_addr, currentAddress->ai_addrlen) == SUCCESS)
			{
				PrintCreatingListenerInfo(currentAddress->ai_addr, currentAddress->ai_addrlen);
				break;
			}

			CloseSocket(listenSocket);
			listenSocket = BAD_SOCKET;
		}
		/*freeaddrinfo(): Frees the address information that was dynamically allocated by the getaddrinfo().*/
		freeaddrinfo(addressResult);

		if (listenSocket != BAD_SOCKET)
		{
			constexpr int LISTEN_BACKLOG = 1024;
			if (listen(listenSocket, LISTEN_BACKLOG) != SUCCESS)
			{
				CloseSocket(listenSocket);
				return BAD_SOCKET;
			}
		}
		return listenSocket;
	}

	sock OpenClientSocket(const char* host, const char* service)
	{
		// Instance hints
		addrinfo hints = {};

		// Set hint that we want to get a TCP STREAM socket and want both IPv4 and IPv6
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_ADDRCONFIG;

		// Get an address result list
		addrinfo* addressResult = nullptr;
		if (const auto foundInformation = ::CanGetAddressInfo(host, service, &hints, &addressResult);
			foundInformation == false)
		{
			return BAD_SOCKET;
		}

		// Walk through this list
		sock clientSocket = BAD_SOCKET;
		constexpr int FAILURE = -1;
		for (addrinfo* currentAddress = addressResult; currentAddress != nullptr; currentAddress = currentAddress->ai_next)
		{
			// get a socket with current setting
			clientSocket = socket(currentAddress->ai_family, currentAddress->ai_socktype, currentAddress->ai_protocol);
			if(clientSocket == BAD_SOCKET)
			{
				continue;
			}

			/* connect() : Establish a connection to a requested socket */
			// ai_addr of current address points to our input address.
			if (connect(clientSocket, currentAddress->ai_addr, currentAddress->ai_addrlen) != FAILURE)
			{
				PrintConnectToServer(currentAddress->ai_addr, currentAddress->ai_addrlen);
				break;
			}

			// if connect is failed, close the opened one and keep iteration
			CloseSocket(clientSocket);
			clientSocket = BAD_SOCKET;
		}
		freeaddrinfo(addressResult);
		return clientSocket;
	}

	void CloseSocket(sock socketHandle)
	{
#ifdef _WIN32
		closesocket(socketHandle);
#else
		close(socketHandle);
#endif
	}



	void PrintConnectToClient(sockaddr_storage client_address, socklen_t socket_address_storage_size) noexcept
	{
		std::array<char, bufferSize> client_hostname{};
		std::array<char, bufferSize> client_port{};
		const auto psocketaddress_information = reinterpret_cast<sockaddr*>(&client_address);
		// Call getnameinfo() to get a string version of the clients (IP:Port) information
		getnameinfo(psocketaddress_information, socket_address_storage_size, &client_hostname.front(), bufferSize, &client_port.front(), bufferSize, 0);
		
		std::cout << "Connected to client at (" << client_hostname.data() << ", " << client_port.data() << ") / ";
		
		getnameinfo(psocketaddress_information, socket_address_storage_size, &client_hostname.front(), bufferSize, &client_port.front(), bufferSize, NI_NUMERICHOST);
		std::cout << '(' << client_hostname.data() << ", " << client_port.data() << ")\n\n";
	}

	void PrintConnectToServer(sockaddr* sock_address_information, socklen_t socket_address_storage_size) noexcept
	{
		std::array<char, bufferSize> hostname{};
		std::array<char, bufferSize> port{};
		getnameinfo(sock_address_information, socket_address_storage_size, &hostname.front(), bufferSize, &port.front(), bufferSize, NI_NUMERICSERV);
		std::cout << "Connected to sever at (" << hostname.data() << ", " << port.data() << ") / ";
		getnameinfo(sock_address_information, socket_address_storage_size, &hostname.front(), bufferSize, &port.front(), bufferSize, NI_NUMERICHOST);
		std::cout << '(' << hostname.data() << ", " << port.data() << ")\n\n";
	}

	void PrintCreatingListenerInfo(sockaddr* sock_address_information, socklen_t socket_address_storage_size) noexcept
	{
		std::array<char, bufferSize> hostname{};
		std::array<char, bufferSize> port{};
		getnameinfo(sock_address_information, socket_address_storage_size, &hostname.front(), bufferSize, &port.front(), bufferSize, NI_NUMERICHOST);
		std::cout << "Connected to sever at (" << hostname.data() << ", " << port.data() << ") / ";
		getnameinfo(sock_address_information, socket_address_storage_size, &hostname.front(), bufferSize, &port.front(), bufferSize, NI_NUMERICSERV);
		std::cout << '(' << hostname.data() << ", " << port.data() << ")\n\n";
	}


	bool GetInputWithBuffer(const SocketLib::sock socket, std::string& str)
	{
		std::array<char, bufferSize> buffer{};
		static std::string data{};
		
		str.clear();
		while (str.empty() == true)
		{
			if (const long long bytes_received = recv(socket, &buffer.front(), bufferSize, 0);
				bytes_received > 0)
			{

				for (long long i = 0; i < bytes_received; ++i)
				{
					// Parse received data
					switch (buffer.at(i))
					{
					case '\0':
						//TODO: DEBUG code
						if (data.empty() == false)
						{
							str = data;
						}
						data.clear();
						break;
					default:
						data.push_back(buffer.at(i));
						break;
					}
				}
			}
			else
			{
				// TODO: DEBUG PRINTING
				std::cout << "Writer detection is detected!\n";
				return false;
			}
		}

		return true;
	}

	bool SendString(SocketLib::sock socket, std::string msg)
	{
		std::cout << "msg = " << msg << std::endl;	// DEBUG
		std::string buffer{};
		std::string message = '\0' + msg + '\0';
		std::cout << "message = " << message << std::endl;	// DEBUG

		for (size_t i = 0; i < message.size(); i++)
		{
			if (buffer.size() >= SEND_BUFFER_MAX)
			{
				// Going to send
				std::cout << "send : buffer = " << buffer << std::endl;	// DEBUG
				if(const bool is_success = SocketLib::SEND(socket, buffer);
					is_success == false)
				{
					return false;
				}
				buffer.clear();
			}
			buffer.push_back(message.at(i));
		}

				std::cout << "send : buffer = " << buffer << std::endl;	// DEBUG
		if(const bool is_success = SocketLib::SEND(socket, buffer);
			is_success == false)
		{
			return false;
		}
		
		return true;
	}

	bool SEND(SocketLib::sock socket, const std::string& packed_msg)
	{
		if (packed_msg.size() > SEND_BUFFER_MAX)
		{
			return false;
		}
		const int bytes_sent = send(socket, packed_msg.c_str(), packed_msg.size(), 0);

		// if transferring is weird,
		if (bytes_sent <= 0)
		{
			std::cout << "errno : " << errno << std::endl;
			// print debug message
			std::cout << ((bytes_sent < 0) ? "Failed to send message\n" : "Nothing sent\n");
			return false;
		}
		return true;
	}
}


namespace
{
	bool CanGetAddressInfo(const char* host, const char* service, const addrinfo* hints, addrinfo** results)
	{
		// try to get a information of given address
		// If failed, print error string and return false
		if (const int error = getaddrinfo(host, service, hints, results);
			error)
		{
			std::cerr << "getaddrinfo error: " << gai_strerror(error) << std::endl;
			return false;
		}

		return true;
	}
}