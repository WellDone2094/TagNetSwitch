//
// Created by Andrea Benfatti on 04/04/16.
//

#ifndef TAGSWITCH_BUFFERMANAGER_H
#define TAGSWITCH_BUFFERMANAGER_H

#include <queue>
#include <mutex>

#define BUFFER_SIZE 1024
#define N_BUFFER 100

struct Buffer {
    union {
        struct Buffer * next;
        char data[BUFFER_SIZE];
    };
};

class Packet;
class BufferManager {
public:
    BufferManager();
    Packet* allocate();
    void release(Packet* p);

private:
    int size=N_BUFFER;
    Buffer * free_list;
//    std::queue<unsigned char*> buffer_queue;
    std::mutex mutex;

};


#endif //TAGSWITCH_BUFFERMANAGER_H
