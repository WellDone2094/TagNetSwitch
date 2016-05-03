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
#include "predicate.h"

class Switch{
public:
    Switch();

    bool running = true;

    void worker();

    const std::string executeCommand(std::string s);
    const std::string add_interface(int inPort, int outPort, const std::string& ip);
    const std::string list_interfaces();
    const std::string start_interface(int id);
    const std::string stop_interface(int id);
    const std::string add_filter(int tree, int interface, const std::string& filter);
    const std::string quit();

private:
    std::map <int, VirtualInterface*> interfaces;
    SyncQueue<Packet*> packetQueue;
    int lastPort;
    int lastId;
    BufferManager bufferManager;
    SwitchManager switchManager;
    predicate matcher;
    std::thread* worker_t;

};


#endif //TAGSWITCH_SWITCH_H
