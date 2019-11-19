// Copyright (C) 2019 DigiPen Institute of Technology.
// Reproduction or disclosure of this file or its contents without the prior
// written consent of DigiPen Institute of Technology is prohibited.
//
// File Name: sockets.cpp
// Purpose:   To implement the platform specifics needed for berkeley sockets.
// Language:  C++
// Platform:  Visual Studio 16.2.5 w/ C++17, g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
// Project:   CS180 Operating Systems
// Author:    Yoonsoo Kwon, Sinil Gang, Wonju Cho
// Creation date: 11/19/2019

#include "sockets.h"
#include <iostream>

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
    bool can_get_address_info(const char* host, const char* service, const addrinfo* hints, addrinfo** results);
}

namespace sockets
{
    sock open_listen_socket(const char* service, AddressFamily address_family)
    {
        addrinfo hints{};

        // See getaddrinfo()
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html
        // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo?redirectedfrom=MSDN

        hints.ai_socktype = SOCK_STREAM; // set hint that we want a reliable TCP STREAM socket
        hints.ai_flags = AI_PASSIVE; // set hint that we want a PASSIVE socket suitable for binding a socket that will
                                     // `accept()` connections
        switch (address_family)
        {
            case AddressFamily::IPv4:
                hints.ai_family = AF_INET; // set hint to use IPv4 addressing only
                break;
            case AddressFamily::IPv6:
                hints.ai_family = AF_INET6; // set hint to use IPv6 addressing only
                break;
            case AddressFamily::HostPreferred:
                hints.ai_flags |= AI_ADDRCONFIG; // Add a flag to hint that we want both IPv4 and IPv6 if available
                break;
            default: break;
        }

        addrinfo* address_results = nullptr;
        if (const auto found_information = ::can_get_address_info(nullptr, service, &hints, &address_results);
            !found_information)
        {
            return BAD_SOCKET;
        }

        sock          listen_socket{BAD_SOCKET};
        constexpr int SUCCESS = 0;
        for (addrinfo* current_address = address_results; current_address != nullptr;
             current_address           = current_address->ai_next)
        {
            // socket()
            // https://pubs.opengroup.org/onlinepubs/7908799/xns/socket.html
            // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket?redirectedfrom=MSDN
            listen_socket = socket(current_address->ai_family,current_address->ai_socktype,current_address->ai_protocol);

            if (listen_socket == BAD_SOCKET)
                continue; // Socket failed, try the next

            // bind()
            // https://pubs.opengroup.org/onlinepubs/7908799/xns/bind.html
            // https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-bind
            if (bind(listen_socket,current_address->ai_addr, current_address->ai_addrlen) == SUCCESS)
            {
                break;
            }

            // bind() failed so close the opened socket and try the next
            close_socket(listen_socket);
            listen_socket = BAD_SOCKET;
        }

        // freeaddrinfo()
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html
        // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo
        freeaddrinfo(address_results);

        if (listen_socket != BAD_SOCKET)
        {
            constexpr int LISTEN_BACKLOG = 1024;
            // listen()
            // https://pubs.opengroup.org/onlinepubs/7908799/xns/listen.html
            // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen?redirectedfrom=MSDN
            if (listen(listen_socket, LISTEN_BACKLOG) != SUCCESS)
            {
                close_socket(listen_socket);
                return BAD_SOCKET;
            }
        }

        return listen_socket;
    }

    sock open_client_socket(const char* host, const char* service)
    {
        addrinfo hints = {};

        // See getaddrinfo()
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html
        // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo?redirectedfrom=MSDN

        hints.ai_socktype = SOCK_STREAM; // set hint that we want a reliable TCP STREAM socket
        hints.ai_flags    = AI_ADDRCONFIG; // Add a flag to hint that we want both IPv4 and IPv6 if available

        addrinfo* address_results;
        if (const auto found_information = ::can_get_address_info(host, service, &hints, &address_results);
            !found_information)
        {
            return BAD_SOCKET;
        }

        sock          client_socket = BAD_SOCKET;
        constexpr int FAILURE       = -1;
        for (addrinfo* current_address = address_results; current_address != nullptr;
             current_address           = current_address->ai_next)
        {
            // socket()
            // https://pubs.opengroup.org/onlinepubs/7908799/xns/socket.html
            // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket?redirectedfrom=MSDN
            client_socket = socket(current_address->ai_family,current_address->ai_socktype,current_address->ai_protocol);

            if (client_socket == BAD_SOCKET)
                continue; // Socket failed, try the next

            // connect()
            // https://pubs.opengroup.org/onlinepubs/7908799/xns/connect.html
            // https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect
            if (connect(client_socket,current_address->ai_addr, current_address->ai_addrlen) != FAILURE)
            {
                break;
            }

            // connect() failed so close the opened socket and try the next
            close_socket(client_socket);
            client_socket = BAD_SOCKET;
        }

        // freeaddrinfo()
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html
        // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo
        freeaddrinfo(address_results);
        return client_socket;
    }

    void close_socket(sock socket_handle)
    {
#ifdef _WIN32
        closesocket(socket_handle);
        // https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-closesocket
#else
        close(socket_handle);
        // https://pubs.opengroup.org/onlinepubs/7908799/xns/close.html
#endif
    }
}
namespace
{
    bool can_get_address_info(const char* host, const char* service, const addrinfo* hints, addrinfo** results)
    {
        //  getaddrinfo()
        // https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html
        // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo?redirectedfrom=MSDN

        if (const int error = getaddrinfo(host, service, hints, results); error)
        {
            // gai_strerror()
            // https://pubs.opengroup.org/onlinepubs/9699919799/functions/gai_strerror.html
            // https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-gai_strerrora
            std::cerr << "getaddrinfo error: " << gai_strerror(error) << '\n';
            return false;
        }
        return true;
    }
// ========================================================================
//  This class is designed to be a global singleton that initializes
//  and shuts down Winsock.
// ========================================================================
#ifdef _WIN32
    class WindowsSocketSystem
    {
    public:
        WindowsSocketSystem()
        {    
            WORD wVersionRequested;
            wVersionRequested = MAKEWORD(2,2);
            // https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsastartup
            WSADATA windows_socket_data{};
            WSAStartup(wVersionRequested, &windows_socket_data);
        }
        ~WindowsSocketSystem()
        {
            WSACleanup();
            // https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsacleanup
        }
    };
    WindowsSocketSystem g_system;
#endif
}