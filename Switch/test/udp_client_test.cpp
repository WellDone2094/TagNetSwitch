//
// Created by Andrea Benfatti on 23/03/16.
//


#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>	        /* for memcpy */
#include <netdb.h>
#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include "../filter.hh"
#include "../hashFunction.h"

#define BUFSIZE 2048

void receiver(){
    struct sockaddr_in address;
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);
    int socketDescriptor;
    if((socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket receiver");
        return; //TODO
    }
    if (bind(socketDescriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return; //TODO
    }

    int recvlen;
    socklen_t addrlen = sizeof(address);

    std::cout << "server waiting on 5000" << std::endl;
    while(true){
        /* byte received */
        char buffer[1024];


        recvlen = recvfrom(socketDescriptor, buffer, sizeof(char)*1024, 0, (struct sockaddr *)&address, &addrlen);

        std::cout << "received" << std::endl;

        if (recvlen > 0) {
            std::cout << buffer << std::endl;
        }
    }
}

int main() {
    std::thread rec(&receiver);

    for(;;) {
        struct hostent *hp;     /* host information */
        struct sockaddr_in servaddr;    /* server address */
        struct sockaddr_in remaddr;             /* remote address */
        char *host = "127.0.0.1";
        char my_message[BUFSIZE];
        unsigned char buf[BUFSIZE];             /* receive buffer */
        socklen_t addrlen = sizeof(remaddr);    /* length of addresses */
        int recvlen;                            /* # bytes received */
        int fd;
        int port;

        /* receive socket from system */
        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("cannot create socket");
            return 0;
        }
        std::cout << "insert port number: ";
        std::cin >> port;
        /* fill in the server's address and data */
        memset((char *) &servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);

        getchar();
        std::cout << "insert message:" << std::endl;
        std::string line;
        std::getline(std::cin, line);


        /* add tree number and descriptor to the message */
        int16_t tree = 0;

        filter_t f;
        f.clear();
        std::string s = "usi";
        for(int i=0; i<7; ++i)
            f.set_bit(hash(i, s.c_str(), s.c_str()+s.length())%192);

        strncpy(my_message, (char*)&tree, 2);
        memcpy(my_message+2, f.b, 24);
//        strncpy(my_message+2, (char*)&f.b[0], 8);
//        strncpy(my_message+10, (char*)&f.b[1], 8);
//        strncpy(my_message+18, (char*)&f.b[2], 8);
        strcpy(my_message+26, line.c_str());

//        std::cout << std::bitset<64>(b[0]) << std::bitset<64>(b[1]) << std::bitset<64>(b[2]) << std::endl;

        f.write_ascii(std::cout) << std::endl;
        for (int i = 0; i < 24; ++i)
            for(unsigned char mask = 1; mask != 0; mask <<= 1)
                std::cout << ((my_message[i+2] & mask) ? '1' : '0');
        std::cout << my_message << std::endl;


        /* look up the address of the server given its name */
        hp = gethostbyname(host);
        if (!hp) {
            std::cerr << "could not obtain address of " << host << std::endl;
            return 0;
        }

        /* put the host's address into the server address structure */
        memcpy((void *) &servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
        servaddr.sin_addr.s_addr = inet_addr(host);

        /* send a message to the server */
        if (sendto(fd, my_message, 1024, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            perror("sendto failed");
            return 0;
        }

    }
}
