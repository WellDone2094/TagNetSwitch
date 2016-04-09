//
// Created by Andrea Benfatti on 04/04/16.
//

#include <iostream>
#include "BufferManager.h"

Packet* BufferManager::allocate() {
    unsigned char* buffer;
    std::lock_guard<std::mutex> lk(mutex);
    if(buffer_queue.empty()){
        buffer = new unsigned char[BUFFER_SIZE];
    }else{
        buffer = buffer_queue.front();
        buffer_queue.pop();
    }
    return new Packet(buffer);
}

void BufferManager::release(Packet *p) {
    std::lock_guard<std::mutex> lk(mutex);
    buffer_queue.push(p->buffer);
    delete p;
}
