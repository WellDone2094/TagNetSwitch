//
// Created by Andrea Benfatti on 23/03/16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include "VirtualInterface.h"


VirtualInterface::VirtualInterface(int id, u_int32_t addressReceiver, int portReceiver, u_int32_t addressSender, int portSender, SyncQueue<Packet* >* q, BufferManager* bufferManager) {

    this->id = id;
    this->bufferManager = bufferManager;
    this->packetQueue = q;
    this->portReceiver = portReceiver;
    this->portSender = portSender;


    /* set address */
    memset((char *)&this->addressReceiver, 0, sizeof(this->addressReceiver));
    this->addressReceiver.sin_family = AF_INET;
    this->addressReceiver.sin_addr.s_addr = addressReceiver;
    this->addressReceiver.sin_port = htons(portReceiver);

    memset((char *)&this->addressSender, 0, sizeof(this->addressSender));
    this->addressSender.sin_family = AF_INET;
    this->addressSender.sin_addr.s_addr = addressSender;
    this->addressSender.sin_port = htons(portSender);

}

void VirtualInterface::start() {
    /* receive socket from system */
    if((socketDescriptorReceiver = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket receiver");
        return; //TODO
    }

    if((socketDescriptorSender = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket sender");
        return; //TODO
    }

    /* bind the address */
    if (bind(socketDescriptorReceiver, (struct sockaddr *)&addressReceiver, sizeof(addressReceiver)) < 0) {
        perror("bind failed");
        return; //TODO
    }

    treceiver = std::thread(&VirtualInterface::receiver, this);
    tsender = std::thread(&VirtualInterface::sender, this);
}


void VirtualInterface::sendPakcet(Packet* packet) {
    sendQueue.push(packet);
}

void VirtualInterface::receiver() {
    int recvlen;

    std::cout << "server waiting on " << portReceiver << std::endl;
    while(running){
        /* byte received */
        Packet* p = bufferManager->allocate();

        recvlen = recvfrom(socketDescriptorReceiver, p->buffer, BUFFER_SIZE, 0, (struct sockaddr *)&remAddress, &addrlen);

        if (recvlen > 0) {
            p->buffer[recvlen] = 0;
            packetQueue->push(p);
        } else {
            bufferManager->release(p);
        }
    }
}

void VirtualInterface::sender() {
    while(running){
        Packet* p = sendQueue.pop();
        if (p!= nullptr) {
            if (sendto(socketDescriptorSender, p->buffer, BUFFER_SIZE, 0, (struct sockaddr *)&addressSender, sizeof(addressSender)) < 0) {
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
    std::cout << "closed" << std::endl;
}

