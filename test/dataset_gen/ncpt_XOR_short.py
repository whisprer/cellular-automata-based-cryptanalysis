import sys

def xor_encrypt(data, key):
    out = bytearray(len(data))
    key_len = len(key)
    for i in range(len(data)):
        out[i] = data[i] ^ key[i % key_len]
    return out

key_str = "LOREM_IPSUM"
key_bytes = key_str.encode('utf-8')

with open(sys.argv[1], "rb") as fin, open(sys.argv[2], "wb") as fout:
    plaintext = fin.read()
    encrypted = xor_encrypt(plaintext, key_bytes)
    fout.write(encrypted)