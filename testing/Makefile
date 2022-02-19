CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

all: fifo-test uri-test
fifo-test: test.c ../ABC_fifo.h
	$(CC) test.c -o fifo-test -Iinclude -Llib -lSDL2-2.0.0 -g

uri-test: uri.c ../ABC_uri.h
	$(CC) -Wall -Wextra -Wpedantic uri.c -o uri-test -g -fsanitize=address -fsanitize=leak

clean:
	rm -rf fifo-test uri-test
