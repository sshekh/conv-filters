CC=g++-10
CFLAGS= -std=gnu++20 -O3
LIB= -Wl,-rpath,/home/manycore6/lib
LDFLAGS= -ltbb -lm
DEBUG=-g

all: main

main: conv2d_layer.o filter.o  main.cpp
	$(CC) $(LIB) $(CFLAGS) main.cpp -o main $(LDFLAGS)

conv2d_layer.o: conv2d_layer.hpp filter.o
	$(CC) $(CFLAGS) conv2d_layer.hpp -o conv2d_layer.o

filter.o: filter.hpp
	$(CC) $(CFLAGS) filter.hpp -o filter.o

# Cuboid.o: Cuboid.hpp
# 	$(CC) $(CFLAGS) Cuboid.hpp -o Cuboid.o

clean:
	rm *.o main
