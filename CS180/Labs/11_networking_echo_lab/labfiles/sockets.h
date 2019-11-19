// Copyright (C) 2019 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: sockets.h
// Purpose:   To have one header file to expose berkeley sockets, smooth out the platform subtle differences and to
//            provide helper functions that help you create common TCP/IP sockets.
// Language:  C++
// Platform:  Visual Studio 16.2.5 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Rudy Castan <rudy.castan@digipen.edu>
// Creation date: 2018/11/08
#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>   // almost everything is contained in one file.
#else                   // UNIX/Linux
#include <arpa/inet.h>  // contains all inet_* functions
#include <errno.h>      // contains the error functions
#include <fcntl.h>      // file control
#include <netdb.h>      // for DNS - gethostbyname()
#include <netinet/in.h> // IPv4 and IPv6 stuff
#include <sys/socket.h> // header containing socket data types and functions
#include <sys/types.h>  // header containing all basic data types and typedefs
#include <unistd.h>     // for gethostname()
#endif


namespace sockets
{
#ifdef _WIN32

    typedef SOCKET sock; // Although sockets are int's on unix,
                         // windows uses it's own typedef of
                         // SOCKET to represent them. If you look
                         // in the Winsock2 source code, you'll see
                         // that it is just a typedef for unsigned int, but
                         // there is absolutely no guarantee that it
                         // won't change in a later version.
                         // therefore, in order to avoid confusion,
                         // this library will create it's own basic
                         // socket descriptor typedef

    // Use Window's macro for invalid socket, so that it matches
    // with their SOCKET type.
    constexpr sock BAD_SOCKET = INVALID_SOCKET;

#else // UNIX/Linux
    using sock = int;
    constexpr sock BAD_SOCKET = ~0;
#endif

    enum class AddressFamily
    {
        IPv4,         // To use IP addresses like 220.69.157.148
        IPv6,         // To use IP addresses like fe80::4176:ad1b:9b82:33b7%7
        HostPreferred // Use what the host machine prefers
    };

    /**
     * \brief Open a listening socket, to be used by servers to accept client connection requests.
     * \param service which port service to create the socket on
     * \param address_family use this to pick which version of the IP you want the listening socket to work with.
     * \return socket with the host's ip address and the provided port value or BAD_SOCKET on failure.
     */
    sock open_listen_socket(const char* service, AddressFamily address_family = AddressFamily::HostPreferred);


    /**
     * \brief Open a client socket, to be used by clients to make a connection request and then continue to use it to
     * send/receive data from the server.
     * \param host provide a way to address the machine to connect to, which could be an ip address or domain name.
     * \param service provide the port/service associated with the process to connect to on the   machine to  connect to.
     * \return socket with the server's ip address and the provided port value or BAD_SOCKET on failure.
     */
    sock open_client_socket(const char* host, const char* service);


    /**
     * \brief Use this to close and free up the resources associated with the provided socket.
     * \param socket_handle should be a currently active socket
     */
    void close_socket(sock socket_handle);
}
