CFLAGS=-std=c99 -Wall -Wpedantic -Wextra -Wshadow -Wconversion -Wno-sign-compare
CC=gcc

all:
	$(CC) $(CFLAGS) main.c -o main
