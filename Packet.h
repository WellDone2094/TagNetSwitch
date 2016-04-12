//
// Created by Andrea Benfatti on 02/04/16.
//

#ifndef TAGSWITCH_PACKAGE_H
#define TAGSWITCH_PACKAGE_H


#include "BufferManager.h"

class Packet {
public:
    Packet(Buffer* b);
    int inputInterface=-1;
    Buffer *buffer;

};


#endif //TAGSWITCH_PACKAGE_H
