P = 131071

def hash(x, s):
    h = x
    for c in s:
        h = (x*h + ord(c)) % P
    return h
