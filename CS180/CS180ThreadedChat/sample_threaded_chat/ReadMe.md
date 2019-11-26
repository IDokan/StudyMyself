# How To Use

## invoke chat_server <port or service name>
The server will run and be associated with the provided port or service name.

Examples: 
* ./chat_server 4003
* chat_server.exe 12345
* ./chat_server echo
* chat_server.exe 7


## invoke a client
    
### chat_browser <server ip address> <port or service name>
The browser will be used to see the conversation. You will need to provide the server's host address and port number.

Examples:
* ./chat_browser 192.168.0.8 4003
* chat_browser.exe rudy-Meerkat echo

### chat_writer <server ip address> <port or service name> [nickname]
    
The writer will be used to contribute to the conversation. You will need to provide the server's host address, port number and optionally provide a nickname to identify yourself in the chat.

Examples:
* ./chat_writer 192.168.0.8 4003 rudy1
* chat_writer.exe 192.168.0.6 echo rudy2


## general info

The server is setup that any number of clients can connect to it, though I have not done any stress testing past testing 8 connected clients. 

The only way to stop the server is to kill it manually.
The browser client should auto close if the server shuts down or can be killed manually.
The writer client can be closed by issuing a \quit command.

To utilize more than one machine on the same network you can look up their IP address info with the following:

Windows -- `ipconfig`
	* IPv4 Address. . . . . . . . . . . : `192.168.0.6`
	* Link-local IPv6 Address . . . . . : `fe80::4176:ad1b:9b82:33a7%7`
Ubunutu -- `ifconfig`
    * inet `192.168.0.8`
    * inet6 `fe80::de28:85b3:6e99:3fad`
