//
// Created by Andrea Benfatti on 23/03/16.
//

#ifndef TAGSWITCH_VITRUALINTERFACE_H
#define TAGSWITCH_VITRUALINTERFACE_H

#include <thread>
#include <netinet/in.h>
#include "Packet.h"
#include "SyncQueue.h"
#include "BufferManager.h"


class VirtualInterface {
public:
    VirtualInterface(int id, u_int32_t addressReceiver, int portReceiver, u_int32_t addressSender, int portSender, SyncQueue<Packet* >* q, BufferManager* bufferManager);

    void start();
    void stop();
    int getPort();
    void sendPakcet(Packet* packet);
    std::string toString();

private:
    int id;
    in_addr_t addressReceiver;
    in_addr_t addressSender;
    int portReceiver;
    int portSender;
    int socketDescriptorReceiver;
    int socketDescriptorSender;

    std::thread tsender;
    std::thread treceiver;
    bool running = true;

    SyncQueue<Packet* >* packetQueue;
    SyncQueue<Packet* > sendQueue;

    BufferManager* bufferManager;


    void receiver();
    void sender();
};


#endif //TAGSWITCH_VITRUALINTERFACE_H
