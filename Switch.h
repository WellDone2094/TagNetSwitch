//
// Created by Andrea Benfatti on 02/04/16.
//

#ifndef TAGSWITCH_SWITCH_H
#define TAGSWITCH_SWITCH_H


#include <map>
#include "VirtualInterface.h"
#include "SyncQueue.h"
#include "BufferManager.h"
#include "SwitchManager.h"

class Switch {
public:
    Switch();
    void addInterface(int port, std::string ip);
    void executeCommand(std::string s);
    void printer();

private:
    std::map <int, VirtualInterface*> interfaces;
    SyncQueue<Packet*> packetQueue;
    int lastPort;
    int lastId;
    BufferManager bufferManager;
    SwitchManager switchManager;

};


#endif //TAGSWITCH_SWITCH_H
