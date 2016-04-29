//
// Created by Andrea Benfatti on 02/04/16.
//

#include <iostream>
#include <arpa/inet.h>
#include <zconf.h>
#include "Switch.h"

#define N_FILTERS 10


Switch::Switch() : matcher(N_FILTERS){
    lastPort = 4000;
    lastId = 0;
    std::thread * t1 = new std::thread(&Switch::worker, this);

    switchManager.add_method("list_interfaces", new SwitchMethod(this, &Switch::list_interfaces));
    switchManager.add_method("add_interface", new SwitchMethod(this, &Switch::add_interface));
    switchManager.add_method("start_interface", new SwitchMethod(this, &Switch::start_interface));
    switchManager.add_method("stop_interface", new SwitchMethod(this, &Switch::stop_interface));
    switchManager.add_method("add_filter", new SwitchMethod(this, &Switch::add_filter));
}

const std::string Switch::add_interface(int port, const std::string& ip) {
    lastId++;
    VirtualInterface* i = new VirtualInterface(lastId, INADDR_ANY, lastPort++, inet_addr(ip.c_str()), port ,&packetQueue, &bufferManager);
    interfaces[lastId] = i;
    i->start();
    return i->toString() + "\n";
}

void Switch::worker() {
    for(;;){
        Packet* p = packetQueue.pop();
        p->interfaces = &interfaces;
        p->descriptor.printStr();
        matcher.match(p->descriptor, *(p->tree), *p);
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

const std::string Switch::add_filter(int tree, int interface, const std::string& filter_s) {
    filter_t f(filter_s);
    std::cout << tree << " " << interface << " " << filter_s << std::endl;
    matcher.add(f, (tree_t) tree, (interface_t) interface);
    return "filter added";
}

// 000101110011011000100101001000010001100100100110010000100011000110000011011000101001010100100001001101010010010100010110001000100011010110000000001001100010100100010111001101010110001001100011
// 001101011000000000100110001010010001011100110101011000100110001110000011011000101001010100100001001101010010010100010110001000100001011100110110001001010010000100011001001001100100001000110001


