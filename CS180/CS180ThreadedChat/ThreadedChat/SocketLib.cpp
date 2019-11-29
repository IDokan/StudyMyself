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

#include "SocketLib.h"

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

        switch (address_family)
        {
        case AddressFamily::IPv4:
            /* code */
            break;
        case AddressFamily::IPv6:
            /* code */
            break;
        case AddressFamily::HostPreferred:
            /* code */
            break;
        default:
            break;
        }
    }
}