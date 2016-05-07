def setBit(arr, n):
    i = 23-(n//8)
    bit = n%8
    arr[i] = arr[i] | 2**bit
