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
    void printer();

    const std::string executeCommand(std::string s);
    const std::string addInterface(int port, const std::string& ip);
    const std::string list_interfaces();
    const std::string start_interface(int id);
    const std::string stop_interface(int id);

private:
    std::map <int, VirtualInterface*> interfaces;
    SyncQueue<Packet*> packetQueue;
    int lastPort;
    int lastId;
    BufferManager bufferManager;
    SwitchManager switchManager;

};


#endif //TAGSWITCH_SWITCH_H
