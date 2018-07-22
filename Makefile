CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra
OBJECTS = main.o test.o 
SOURCE  = src
BUILD  = build

all: test
test: test.c
	$(CC) test.c -o test -I$(SOURCE)/include -Llib -lSDL2-2.0.0 -g

clean:
	    rm -rf $(BUILD)/*.o dome ${SOURCE}/lib/wren ${SOURCE}/lib/libwren.a
