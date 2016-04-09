//
// Created by Andrea Benfatti on 02/04/16.
//

#include <algorithm>
#include "Packet.h"

Packet::Packet(unsigned char *buf) {
    this->buffer = buf;
    this->inputInterface = inputInterface;
}

