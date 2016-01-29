#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> /* memset */

typedef struct _coord {
	int row;
	int slot;
} coord;

typedef struct _server {
	unsigned int id;
	unsigned int sz;
	unsigned int pw;
	unsigned int pool;
	coord co;
} server;


static int sortsrvpps(const void *srv1vp, const void *srv2vp);
static int sortsrvsz(const void *srv1vp, const void *srv2vp);
static int sortsrvid(const void *srv1vp, const void *srv2vp);
static int mini(const void *a, const void *b);
void printsrvlist(server *srvl[], unsigned int len);
void printresultlist(server *srvl[], unsigned int len);
int tryInsert(server *srv, char *row, unsigned int rlen);
void printrow(const unsigned int rlen, char rows[rlen]);
void printrows(const unsigned int rlen, const unsigned int num, char rows[][rlen]);
#endif // _MAIN_H
