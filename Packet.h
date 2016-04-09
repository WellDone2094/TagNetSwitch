//
// Created by Andrea Benfatti on 02/04/16.
//

#ifndef TAGSWITCH_PACKAGE_H
#define TAGSWITCH_PACKAGE_H


class Packet {
public:
    Packet(unsigned char *buf);
    int inputInterface=-1;
    unsigned char *buffer;

};


#endif //TAGSWITCH_PACKAGE_H
