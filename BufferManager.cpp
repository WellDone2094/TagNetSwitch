//
// Created by Andrea Benfatti on 04/04/16.
//

#include <iostream>
#include "BufferManager.h"
#include "Packet.h"

Packet* BufferManager::allocate() {
    Buffer* buffer;
    std::lock_guard<std::mutex> lk(mutex);
    if(free_list!= nullptr){
        buffer = free_list;
        free_list = free_list->next;
    }else{
        buffer = new Buffer;
    }
    return new Packet(buffer);
}

void BufferManager::release(Packet* p) {
    std::lock_guard<std::mutex> lk(mutex);
    p->buffer->next = free_list;
    free_list = p->buffer;
    //delete p;
}

BufferManager::BufferManager(){
    free_list = nullptr;
    for (int i = 0; i < size; ++i) {
        Buffer* b = new Buffer;
        b->next = free_list;
        free_list = b;
    }
}


