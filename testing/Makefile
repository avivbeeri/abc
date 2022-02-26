CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra

all: uri-ref uri-test fifo-test

uri-test: uri.c ../ABC_uri.h
	$(CC) -Wall -Wextra -Wpedantic uri.c -o uri-test -g -fsanitize=address -fsanitize=leak

uri-ref: uri_ref.c
	$(CC) -Wall -Wextra -Wpedantic uri_ref.c -o uri-ref -g -fsanitize=address -fsanitize=leak

fifo-test: fifo-test.c ../ABC_fifo.h
	$(CC) fifo-test.c -o fifo-test -Iinclude -Llib -lSDL2-2.0.0 -g

clean:
	rm -rf fifo-test uri-test
