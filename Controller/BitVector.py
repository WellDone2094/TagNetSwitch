class BitVector:
    def __init__(self, size):
        self.size = size
        self.arr = [0x00 for i in range(size//8)]

    def setBit(self, n):
        i = (self.size//8-1)-(n//8)
        bit = n%8
        arr[i] = arr[i] | 2**bit

    def getBytes(self):
        return bytes(arr)

    def setTag(self, s):
        
