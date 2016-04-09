//
// Created by Andrea Benfatti on 23/03/16.
//


#include "VirtualInterface.h"
#include "SyncQueue.h"
#include "Switch.h"
#include "SwitchManager.h"
#include <queue>
#include <iostream>
#include <zconf.h>


void asdf(int a, std::string s){
    std::cout << "asdfasdf " << a << s << std::endl;
}

int main() {

    Switch s;
    s.addInterface(5000, "127.0.0.1");
    for (std::string line; std::getline(std::cin, line);) {
        s.executeCommand(line);
    }

    return 0;
}
