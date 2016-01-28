#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct _server {
	int sz;
	int pw;
} server;

int main()
{
	FILE *input = fopen("dc.in", "ro");

	/* parse file */
	int numRows, slotsPRow, numUnav, numPools, numSrv;
	fscanf(input, "%d %d %d %d %d\n", &numRows, &slotsPRow, &numUnav, &numPools, &numSrv);

	printf( "== INPUT ==\n"
		"number of rows:\t\t%d\n"
		"slots per row:\t\t%d\n"
		"number of unav servers:\t%d\n"
		"number of pools:\t%d\n"
		"number of servers:\t%d\n"
		"== END ==\n",
		numRows, slotsPRow, numUnav, numPools, numSrv);

	intptr_t unavSlots[numUnav];
	int yrow, xslot;
	for (int i = 0; i < numUnav; i++) {
		fscanf(input, "%d %d\n", &yrow, &xslot);
		printf("y-row: %d, x-slot: %d\n", yrow, xslot);
	}

	server *servers[numSrv];
	int size, power;
	for (int j = 0; j < numSrv; j++) {
		fscanf(input, "%d %d\n", &size, &power);

		server *srv = malloc(sizeof(server));
		srv->sz = size;
		srv->pw = power;
		servers[j] = &srv;

		printf("size (slots needed): %d, power-cap provided: %d\n", size, power);
	}

	fclose(input);
}
