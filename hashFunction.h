//
// Created by Andrea Benfatti on 03/05/16.
//

#ifndef TAGSWITCH_HASHFUNCTION_H
#define TAGSWITCH_HASHFUNCTION_H

// I need a prime that is close to a power of two.
//
// P = 2^31 - 1 = 2147483647 works, although I suppose I should
// check that that can be used as an unsigned integer, that is,
// sizeof(unsigned int) >= 4
//
// P = 2^13 - 1 = 8191 also works
// P = 2^17 - 1 = 131071 also works
//
static const unsigned int P = 131071U;

//
// INPUT: string = c[1]c[2]c[3] ... c[N]
// OUTPUT: c[1]*3^N + c[2]*3^(N-1) + ... + c[N-1]*3 + c[N] mod P
//
// generic poly-mod-P hash with a polynomial base X
//
unsigned int hash(unsigned int X, const char* b, const char *e) {
    register unsigned int h = X;
    for (; b != e; ++b) {
        h = (X*h + static_cast<unsigned char>(*b)) % P;
    }
    return h;
}

#endif //TAGSWITCH_HASHFUNCTION_H

