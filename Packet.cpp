//
// Created by Andrea Benfatti on 02/04/16.
//

#include <algorithm>
#include "Packet.h"
#include "VirtualInterface.h"

Packet::Packet(Buffer* buf) {
    this->buffer = buf;
}

void Packet::parse() {
    this->tree = (uint16_t *) buffer->data;
    this->descriptor = filter_t(&buffer->data[2]);
    this->descriptor.printStr();
}

bool Packet::match(const filter_t &filter, tree_t tree, interface_t ifx) {
    std::cout << "tree: " << tree << "\tinterface: " << ifx << std::endl;
    try {
        VirtualInterface *i = interfaces->at(ifx);
        if (i->id != this->inputInterface)
            this->incCopyCounter();
        i->sendPacket(this);
    }catch(std::out_of_range& e){}

    return false;
}

void Packet::decCopyCounter() {
    std::unique_lock<std::mutex> lk(mutex);
    --copyCounter;
}

void Packet::incCopyCounter() {
    std::unique_lock<std::mutex> lk(mutex);
    ++copyCounter;
}

bool Packet::isDeletable() {
    std::unique_lock<std::mutex> lk(mutex);
    return deletable;
}

void Packet::setDeletable(bool b){
    std::unique_lock<std::mutex> lk(mutex);
    deletable = b;
}

