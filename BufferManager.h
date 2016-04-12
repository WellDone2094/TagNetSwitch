//
// Created by Andrea Benfatti on 04/04/16.
//

#ifndef TAGSWITCH_BUFFERMANAGER_H
#define TAGSWITCH_BUFFERMANAGER_H

#include <queue>
#include <mutex>
#include "Packet.h"

#define BUFFER_SIZE 1024
struct Buffer {
    union {
        struct Buffer * next;
        char data[BUFFER_SIZE];
    };
};

class BufferManager {
public:
    Packet* allocate();
    void release(Packet* p);

private:
    Buffer * free_list;
    std::queue<unsigned char*> buffer_queue;
    std::mutex mutex;

};


#endif //TAGSWITCH_BUFFERMANAGER_H
