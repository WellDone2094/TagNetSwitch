//
// Created by Andrea Benfatti on 04/04/16.
//

#ifndef TAGSWITCH_BUFFERMANAGER_H
#define TAGSWITCH_BUFFERMANAGER_H

#include <queue>
#include <mutex>
#include "Packet.h"

#define BUFFER_SIZE 1024

class BufferManager {
public:
    Packet* allocate();
    void release(Packet* p);

private:
    std::queue<unsigned char*> buffer_queue;
    std::mutex mutex;

};


#endif //TAGSWITCH_BUFFERMANAGER_H
