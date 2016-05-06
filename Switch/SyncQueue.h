//
// Created by Andrea Benfatti on 24/03/16.
//

#ifndef TAGSWITCH_SYNCQUEUE_H
#define TAGSWITCH_SYNCQUEUE_H

#include <queue>
#include <mutex>


template <typename T>
class SyncQueue {
public:
    bool empty(){
        std::lock_guard<std::mutex> lk(mutex);
        bool res = q.empty();
        return res;

    };

    void push(T elem){
        std::lock_guard<std::mutex> lk(mutex);
        q.push(elem);
        cv.notify_all();
    };

    T pop(){
        std::unique_lock<std::mutex> lk(mutex);
        while(q.empty()){
            cv.wait(lk);
        }
        T elem = q.front();
        q.pop();
        return elem;
    };

private:
    std::queue<T> q;
    std::mutex mutex;
    std::condition_variable cv;
};


#endif //TAGSWITCH_SYNCQUEUE_H
