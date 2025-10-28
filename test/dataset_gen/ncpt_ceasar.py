import sys

shift = 3  # or anything
with open(sys.argv[1], "rb") as fin, open(sys.argv[2], "wb") as fout:
    data = fin.read()
    out = bytearray((b + shift) % 256 for b in data)
    fout.write(out)