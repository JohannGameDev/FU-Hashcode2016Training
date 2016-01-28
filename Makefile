CFLAGS=-std=c99 -Wall -Wpedantic -Wextra -Wshadow -Wconversion -Wno-sign-compare

all:
	gcc $(CFLAGS) main.c -o main
