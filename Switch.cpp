//
// Created by Andrea Benfatti on 02/04/16.
//

#include <iostream>
#include <arpa/inet.h>
#include <zconf.h>
#include "Switch.h"


Switch::Switch() {
    lastPort = 4000;
    lastId = 0;
    std::thread * t1 = new std::thread(&Switch::printer, this);
}

void Switch::addInterface(int port, std::string ip) {
    lastId++;
    VirtualInterface* i = new VirtualInterface(lastId, INADDR_ANY, lastPort++, inet_addr(ip.c_str()), port ,&packetQueue, &bufferManager);
    interfaces[lastId] = i;
    i->start();
    sleep(1);
    i->stop();
    sleep(1);
    i->start();
}

void Switch::printer() {
    for(;;){
        Packet* p = packetQueue.pop();
        for(std::map<int, VirtualInterface *>::iterator i = interfaces.begin();
            i != interfaces.end();
            i++){
            i->second->sendPakcet(p);
        }
    }
}

void Switch::executeCommand(std::string s) {
    switchManager.executeCmd(s);
}

