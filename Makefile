CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

fifo: test_fifo.c ABC_fifo.h
	$(CC) test_fifo.c -o test-fifo -Iinclude -Llib -lSDL2-2.0.0 -g
bson: test_bson.c ABC_bson.h
	$(CC) test_bson.c -o test-bson -g -Iinclude -Llib

clean:
	    rm -rf test-fifo test-bson
