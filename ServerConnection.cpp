#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <zconf.h>
#include "ServerConnection.h"

ServerConnection::ServerConnection(uint16_t port) : port(port){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cout << "Unable to open listening socket" << std::endl;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(sockfd, (sockaddr*)&server, sizeof(server)) < 0)
        std::cout << "Unable to bind socket" << std::endl;
	if (listen(sockfd, 5) < 0)
		std::cout << "Unable to listen on socket " << std::endl;
    std::cout << "Socket listening on port " << port << std::endl;
}

int ServerConnection::takeConn() {
	int newsocketfd, size = sizeof(sockaddr_in);
	sockaddr_in client;
	newsocketfd = accept(sockfd, NULL, NULL);
	if (newsocketfd < 0)
		return NULL;
	return newsocketfd;
}

ServerConnection::~ServerConnection() {
	std::cout << "Closing server socket " << std::endl;
	close(sockfd);
}
