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


#include <map>
#include "BufferManager.h"
#include "patricia_predicate.hh"
class VirtualInterface;

class Packet : public match_handler {
public:
    Packet(Buffer* b);

    int inputInterface = 1;
    tree_t *tree;
    filter_t descriptor;
    Buffer *buffer;
    std::map <int, VirtualInterface*> *interfaces;
    std::vector<int> output;

    bool decCopyCounter();
    void incCopyCounter();
    bool isDeletable();
    void setDeletable(bool b);


    void parse();
    bool match(const filter_t & filter, tree_t tree, interface_t ifx);

private:

    int copyCounter = 1;
    bool deletable = false;
    std::mutex mutex;
};


#endif //TAGSWITCH_PACKAGE_H
