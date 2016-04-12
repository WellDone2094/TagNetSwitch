//
// Created by Andrea Benfatti on 23/03/16.
//


#include "VirtualInterface.h"
#include "Switch.h"
#include <iostream>
#include <zconf.h>


int main() {

    Switch s;
    for (std::string line; std::getline(std::cin, line);) {
        s.executeCommand(line);
    }

    return 0;
}
