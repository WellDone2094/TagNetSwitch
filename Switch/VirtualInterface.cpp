//
// Created by Andrea Benfatti on 23/03/16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "VirtualInterface.h"


VirtualInterface::VirtualInterface(int id, u_int32_t addressReceiver, int portReceiver, SyncQueue<Packet* >* q, BufferManager* bufferManager) {

    this->id = id;
    this->bufferManager = bufferManager;
    this->packetQueue = q;
    this->portReceiver = portReceiver;
    this->portSender = -1;
    this->addressSender = 0;
    this->addressReceiver = addressReceiver;

}

void VirtualInterface::start() {
    std::lock_guard<std::mutex> lk(mutex);
    if(!running) {
        running = true;
    }else{
        return;
    }

    treceiver = std::thread(&VirtualInterface::receiver, this);
    if(portSender!= -1) {
        senderRunning = true;
        tsender = std::thread(&VirtualInterface::sender, this);
    }
}


void VirtualInterface::sendPacket(Packet* packet) {
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

        recvlen = recv(socketDescriptorReceiver, p->buffer->data, BUFFER_SIZE, 0);

        if (recvlen > 0) {
            p->buffer->data[recvlen] = 0;
            p->inputInterface = id;
            p->parse();
            packetQueue->push(p);
        } else {
            p->setDeletable(true);
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

    while(senderRunning){
        Packet* p = sendQueue.pop();
        if (p!= nullptr) {
            if (sendto(socketDescriptorSender, p->buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sSender, sizeof(sSender)) < 0) {
                std::cout << "send failed" << std::endl;
            }
            bufferManager->release(p);
        }
    }
}


void VirtualInterface::stop() {
    std::lock_guard<std::mutex> lk(mutex);
    if(running) {
        running = false;
    }else {
        return;
    }

    close(socketDescriptorReceiver);
    treceiver.join();
    if(senderRunning) {
        senderRunning = false;
        close(socketDescriptorSender);
        sendQueue.push(nullptr);
        tsender.join();
    }
}

void VirtualInterface::setSenderIpPort(u_int32_t addressSender, int portSender) {
    if (senderRunning) {
        senderRunning = false;
        close(socketDescriptorSender);
        sendQueue.push(nullptr);
        tsender.join();
    }
    this->addressSender = addressSender;
    this->portSender = portSender;
    if(running) {
        senderRunning = true;
        tsender = std::thread(&VirtualInterface::sender, this);
    }
}

std::string VirtualInterface::toString() {
    return "id: "               + std::to_string(id) +
            "\treceive_ip: "    + inet_ntoa({addressReceiver}) +
            "\t\treceive_port: "+ std::to_string(portReceiver) +
            "\t send_ip: "      + inet_ntoa({addressSender}) +
            "\t\tsend_port: "   + std::to_string(portSender) +
            "\trunning: "       + std::to_string(running) +
            "\tsenderRunning: " + std::to_string(senderRunning);
}
