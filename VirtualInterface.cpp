//
// Created by Andrea Benfatti on 23/03/16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "VirtualInterface.h"


VirtualInterface::VirtualInterface(int id, u_int32_t addressReceiver, int portReceiver, u_int32_t addressSender, int portSender, SyncQueue<Packet* >* q, BufferManager* bufferManager) {

    this->id = id;
    this->bufferManager = bufferManager;
    this->packetQueue = q;
    this->portReceiver = portReceiver;
    this->portSender = portSender;
    this->addressSender = addressSender;
    this->addressReceiver = addressReceiver;

}

void VirtualInterface::start() {
    running=true;

    treceiver = std::thread(&VirtualInterface::receiver, this);
    tsender = std::thread(&VirtualInterface::sender, this);
}


void VirtualInterface::sendPakcet(Packet* packet) {
    sendQueue.push(packet);
}

void VirtualInterface::receiver() {
    int recvlen;

    struct sockaddr_in sReceiver;
    /* set address */
    memset((char *)&sReceiver, 0, sizeof(sReceiver));
    sReceiver.sin_family = AF_INET;
    sReceiver.sin_addr.s_addr = INADDR_ANY;//addressReceiver;
    sReceiver.sin_port = htons(portReceiver);


    /* receive socket from system */
    if((socketDescriptorReceiver = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket receiver");
        return; //TODO
    }

    int val = 1;
    setsockopt(socketDescriptorReceiver, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));


    /* bind the address */
    if (bind(socketDescriptorReceiver, (struct sockaddr *)&sReceiver, sizeof(sReceiver)) < 0) {
        perror("bind failed");
        return; //TODO
    }

    while(running){
        /* byte received */
        Packet* p = bufferManager->allocate();

        recvlen = recv(socketDescriptorReceiver, p->buffer, BUFFER_SIZE, 0);

        if (recvlen > 0) {
            p->buffer[recvlen] = 0;
            std::cout << p->buffer << std::endl;
            packetQueue->push(p);
        } else {
            bufferManager->release(p);
        }
    }
}

void VirtualInterface::sender() {
    struct sockaddr_in sSender;
    memset((char *)&sSender, 0, sizeof(sSender));
    sSender.sin_family = AF_INET;
    sSender.sin_addr.s_addr = addressSender;
    sSender.sin_port = htons(portSender);

    if((socketDescriptorSender = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket sender");
        return; //TODO
    }

    while(running){
        Packet* p = sendQueue.pop();
        if (p!= nullptr) {
            if (sendto(socketDescriptorSender, p->buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sSender, sizeof(sSender)) < 0) {
                std::cout << "send failed" << std::endl;
            }else{
                std::cout << "sent " << id << std::endl;
            }
//        bufferManager->release(p);
        }
    }
}

void VirtualInterface::stop() {
    running = false;
    close(socketDescriptorReceiver);
    close(socketDescriptorSender);
    sendQueue.push(nullptr);
    treceiver.join();
    tsender.join();
}

std::string VirtualInterface::toString() {
    return "id: "               + std::to_string(id) +
            "\treceive_ip: "    + inet_ntoa({addressReceiver}) +
            "\t\treceive_port: "  + std::to_string(portReceiver) +
            "\t send_ip: "      + inet_ntoa({addressSender}) +
            "\t\tsend_port: "     + std::to_string(portSender);
}
