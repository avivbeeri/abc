CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

all: test
test: test.c ABC_fifo.h
	$(CC) test.c -o test -Iinclude -Llib -lSDL2-2.0.0 -g

clean:
	    rm -rf test
