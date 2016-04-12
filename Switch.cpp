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

    switchManager.add_method("list_interfaces", new SwitchMethod(this, &Switch::list_interfaces));
    switchManager.add_method("add_interface", new SwitchMethod(this, &Switch::addInterface));
}

void Switch::addInterface(int port, const std::string& ip) {
    lastId++;
    VirtualInterface* i = new VirtualInterface(lastId, INADDR_ANY, lastPort++, inet_addr(ip.c_str()), port ,&packetQueue, &bufferManager);
    interfaces[lastId] = i;
    i->start();
    std::cout << i->toString() << std::endl;
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

void Switch::list_interfaces() {
    for(std::map<int, VirtualInterface *>::iterator i = interfaces.begin();
        i != interfaces.end();
        i++) {
        std::cout << i->second->toString() << std::endl;
    }
}


