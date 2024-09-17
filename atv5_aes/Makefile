all: compile

CC = gcc
SOURCES = src/main.c lib/*.c
CFLAGS = -std=c99 -O2 -Wall -pedantic -Werror
DEFS = -DCBC=1

.PHONY: clean

clean:
	rm -f bin/*

compile:
	$(CC) $(CFLAGS) $(SOURCES) $(DEFS) -o bin/aes
