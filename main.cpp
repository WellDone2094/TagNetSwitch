//
// Created by Andrea Benfatti on 23/03/16.
//


#include <getopt.h>
#include <fstream>
#include <sstream>
#include "VirtualInterface.h"
#include "ServerConnection.h"

#include "Switch.h"

void readConfFile(Switch* s, char* fname){
    std::ifstream infile(fname);
    std::string line;
    while(std::getline(std::cin, line)) {
        s->executeCommand(line);
    }
    std::cout << s->list_interfaces() << std::endl;
}




int main(int argc, char *argv[]) {

    int port = atoi(argv[1]);
    Switch s(port);
    opterr = 0;
    char c;
    while ((c = getopt(argc, argv, "f:")) != -1) {
        switch (c) {
            case 'p':    /* field separator */
                port = atoi(optarg);
                break;
            case 'f':    /* field separator */
                readConfFile(&s, optarg);
                break;
            case '?':
                if (optopt == 'i' || optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
            default:
                break;
        }
    }


    for (std::string line; s.running && std::getline(std::cin, line);) {
        std::cout << s.executeCommand(line) << std::endl;
    }


    return 0;
}



/*
add_interface 5000 127.0.0.1
add_filter 0 1 001101011000000000100110001010010001011100110101011000100110001110000011011000101001010100100001001101010010010100010110001000100001011100110110001001010010000100011001001001100100001000110001

 */
