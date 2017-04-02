#!/bin/bash

# clean the current build
make clean
make

# make image matrices and store them 
python make_mats.py img_mats/out.dat

# run the filter program
./main img_mats/out.dat img_mats/filter_out.dat < filter.txt

# run python program to generate images from matrix
# output written as 0.bmp, 1.bmp ...
python load_img.py img_mats/filter_out.dat out_mats
