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
    VirtualInterface * i = interfaces->at(ifx);
    if (i->id != this->inputInterface)
        i->sendPacket(this);
    return false;
}

