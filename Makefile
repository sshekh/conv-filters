CC=g++ 
CFLAGS= -std=gnu++11 -O2
DEBUG=-g

all: main

main: conv2d_layer.o filter.o main.cpp
	$(CC) $(CFLAGS) main.cpp -o main

conv2d_layer.o: conv2d_layer.hpp filter.o
	$(CC) $(CFLAGS) conv2d_layer.hpp -o conv2d_layer.o

filter.o: filter.hpp
	$(CC) $(CFLAGS) filter.hpp -o filter.o

clean: 
	rm *.o main
