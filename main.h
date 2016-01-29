#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct _server {
	int sz;
	int pw;
} server;


static int sortsrvpps(const void *srv1vp, const void *srv2vp);
static int sortsrvsz(const void *srv1vp, const void *srv2vp);
void printsrvlist(server *srvl[], int len);
#endif // _MAIN_H
