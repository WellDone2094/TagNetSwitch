//
// Created by Andrea Benfatti on 23/03/16.
//


#include "VirtualInterface.h"

#include "Switch.h"


int main() {

    Switch s;
    for (std::string line; std::getline(std::cin, line);) {
        std::cout << s.executeCommand(line) << std::endl;
    }

    return 0;
}



/*
add_interface 5000 127.0.0.1
add_filter 0 1 000101110011011000100101001000010001100100100110010000100011000110000011011000101001010100100001001101010010010100010110001000100011010110000000001001100010100100010111001101010110001001100011


 */
