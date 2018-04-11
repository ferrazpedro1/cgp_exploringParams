CC=g++
CFLAGS=-I.

all: 
	$(CC) -o bin/CGP_ExploringParam src/exploringParam_digital.c src/cgp.c src/cgp.h

clean:
	rm -r bin/*
