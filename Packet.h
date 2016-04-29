//
// Created by Andrea Benfatti on 02/04/16.
//


/*
 * packet format       1024 byte
 *
 * tree-id                2 byte
 * descriptr             24 byte (192 bit)
 * payload              998 byte
 *
 */

#ifndef TAGSWITCH_PACKAGE_H
#define TAGSWITCH_PACKAGE_H
#define DESCRIPTOR_SIZE 192


#include "BufferManager.h"
#include "predicate.h"
class VirtualInterface;

class Packet : public match_handler {
public:
    Packet(Buffer* b);
    int inputInterface = 0;
    tree_t *tree;
    filter_t descriptor;
    Buffer *buffer;
    std::map <int, VirtualInterface*> *interfaces;

    void parse();
    bool match(const filter_t & filter, tree_t tree, interface_t ifx);
};


#endif //TAGSWITCH_PACKAGE_H
