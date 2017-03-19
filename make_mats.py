import numpy as np
from PIL import Image
import pylab
import sys

im_names = ["airplane.png", "lena.png",  "fruits.png"]
gray_imnames = ["barbara.png", "boat.png"]

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
    dirname = 'images/'
    f.write("%d 512 512 3\n" % (len(im_names))) # image tensor
    for im in im_names:
        append_matrix(dirname + im, f)

