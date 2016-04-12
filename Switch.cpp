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
    switchManager.add_method("start_interface", new SwitchMethod(this, &Switch::start_interface));
    switchManager.add_method("stop_interface", new SwitchMethod(this, &Switch::stop_interface));
}

const std::string Switch::addInterface(int port, const std::string& ip) {
    lastId++;
    VirtualInterface* i = new VirtualInterface(lastId, INADDR_ANY, lastPort++, inet_addr(ip.c_str()), port ,&packetQueue, &bufferManager);
    interfaces[lastId] = i;
    i->start();
    return i->toString() + "\n";
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

const std::string Switch::executeCommand(std::string s) {
    return switchManager.executeCmd(s);
}

const std::string Switch::list_interfaces() {
    std::string ret = "";
    for(std::map<int, VirtualInterface *>::iterator i = interfaces.begin();
        i != interfaces.end();
        i++) {
        ret += i->second->toString() + "\n";
    }
    return ret;
}

const std::string Switch::start_interface(int id) {
    if(interfaces.end()==interfaces.find(id))
        return "invalid interface id";

    interfaces[id]->start();
    return "interface started";

}

const std::string Switch::stop_interface(int id) {
    if(interfaces.end()==interfaces.find(id))
        return "invalid interface id";

    interfaces[id]->stop();
    return "interface stoped";
}

