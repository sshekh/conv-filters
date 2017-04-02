#!/usr/bin/python

import numpy as np
from PIL import Image
import pylab
import sys
import os

dirname = 'images/'
#im_names = ["airplane.png", "lena.png",  "fruits.png"]  # 512 * 512 * 3
#gray_imnames = ["barbara.png", "boat.png"] # 512 * 512

def append_matrix(filepath, outf):
    img = Image.open(filepath)
    im = np.asarray(img, dtype='float64')
    #print(im.shape)
    for i in range(im.shape[0]):
        for j in range(im.shape[1]):
            outf.write("%f,%f,%f " %(im[i][j][0], im[i][j][1], im[i][j][2]))
        outf.write('\n')
    outf.write('\n')

if __name__ == "__main__":
    f = open(sys.argv[1], "w")
    depth = 3   # 3 for color, 1 for grey
    foldername = dirname + 'color/' # grey for grey 
    filenames = os.listdir(foldername)
    f.write("%d 512 512 %d\n" % (len(filenames), depth)) # image tensor dimensions
    for filename in filenames:
        append_matrix(foldername + filename, f)

