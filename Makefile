CC=g++ -std=gnu++11
CFLAGS=-Wall -O2
DEBUG=-g

# Call the build routine
all: main

# Build the tree
main: conv2d_layer.o filter.o main.cpp
	$(CC) $(DEBUG) main.cpp -o main

conv2d_layer.o: conv2d_layer.hpp filter.o
	$(CC) $(DEBUG) conv2d_layer.hpp -o conv2d_layer.o

filter.o: filter.hpp
	$(CC) $(DEBUG) filter.hpp -o filter.o

clean: 
	rm *.o main
