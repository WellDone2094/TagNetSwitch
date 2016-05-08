from HashFunction import *

class BitVector:
    def __init__(self, size):
        self.size = size
        self.arr = [0x00 for i in range(size//8)]

    def setBit(self, n):
        i = n//8
        bit = n%8
        self.arr[i] = self.arr[i] | 2**bit

    def getBytes(self):
        return bytes(self.arr)

    def setTag(self, s):
        for i in range(7):
            self.setBit(hash(i,s)%self.size)
