#!/usr/bin/python3

from PIL import Image
import pylab
import sys
import numpy as np

if __name__ == "__main__":
    f = open(sys.argv[1] ,"r")
    shape = list(map(int, f.readline().split()))
    for idx in range(shape[0]):
        im = np.empty(shape=(shape[1], shape[2], shape[3]))
        for i in range(shape[1]):
            row = f.readline().split()
            assert(len(row) == shape[2])  # 512
            for j in range(shape[2]):
                im[i][j] = list(map(float, row[j].split(",")))
        result = Image.fromarray(im.astype(np.uint8))
        result.save(sys.argv[2] + "/" + str(idx) + ".bmp")
        f.readline()    # empty line


