//
// Created by Andrea Benfatti on 02/04/16.
//

#include <iostream>
#include <arpa/inet.h>
#include <zconf.h>
#include "Switch.h"
#include "hashFunction.h"

#define N_FILTERS 10


Switch::Switch(int port) : matcher(N_FILTERS), switchManager(port){
    lastPort = 4000;
    lastId = 0;
    worker_t = new std::thread(&Switch::worker, this);

    switchManager.add_method("list_interfaces", new SwitchMethod(this, &Switch::list_interfaces));
    switchManager.add_method("add_interface", new SwitchMethod(this, &Switch::add_interface));
    switchManager.add_method("set_interface_out", new SwitchMethod(this, &Switch::set_interface_out));
    switchManager.add_method("start_interface", new SwitchMethod(this, &Switch::start_interface));
    switchManager.add_method("stop_interface", new SwitchMethod(this, &Switch::stop_interface));
    switchManager.add_method("add_filter", new SwitchMethod(this, &Switch::add_filter));
    switchManager.add_method("add_tags", new SwitchMethod(this, &Switch::add_tags));
    switchManager.add_method("reset_filters", new SwitchMethod(this, &Switch::reset_filters));
    switchManager.add_method("quit", new SwitchMethod(this, &Switch::quit));
}

const std::string Switch::add_interface(int inPort) {
    lastId++;
    VirtualInterface* i = new VirtualInterface(lastId, INADDR_ANY, inPort, &packetQueue, &bufferManager);
    interfaces[lastId] = i;
    i->start();
    return i->toString() + "\n";
}

const std::string Switch::set_interface_out(int id, int port, const std::string& ip){
    if(interfaces.end()==interfaces.find(id))
        return "invalid interface id";
    interfaces[id]->setSenderIpPort(inet_addr(ip.c_str()), port);
    return interfaces[id]->toString() + "\n";
}

void Switch::worker() {
    while(running){
        Packet* p = packetQueue.pop();
        if(p== nullptr) continue;
        p->interfaces = &interfaces;
        matcher.match(p->descriptor, *(p->tree), *p);
        p->setDeletable(true);
        bufferManager.release(p);
    }

}
const std::string Switch::quit(){
    running = false;
    std::cout << "closing interfaces ... " << std::endl;
    for(std::map<int, VirtualInterface*>::iterator it=interfaces.begin();
            it!=interfaces.end();
            ++it){
        it->second->stop();
        std::cout << "interface " << it->first << " closed" << std::endl;
    }
    std::cout << "closing workers ... " << std::endl;
    packetQueue.push(nullptr);
    worker_t->join();
    std::cout << "worker closed" << std::endl;
    switchManager.quit();
    std::cout << "switchManager closed" << std::endl;
    return "closed";
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
    matcher.add(f, (tree_t) tree, (interface_t) interface);
    return "filter added";
}

const std::string Switch::add_tags(int tree, int interface, std::vector<std::string>& tags) {
    filter_t f;
    for(std::string s : tags){
        for(int i=0; i<7; ++i)
            f.set(hash(i, s.c_str(), s.c_str()+s.length())%192);
    }
    if (matcher.exists_filter(f, tree,interface))
        return "filter already exists";
    matcher.add(f, (tree_t) tree, (interface_t) interface);
    return "filter added";
}

const std::string Switch::reset_filters() {
    matcher.destroy();
    return "filter resetted";
}

