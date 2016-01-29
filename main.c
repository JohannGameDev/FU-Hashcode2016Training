#include "main.h"


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

	/* parse unavailable slots */
	intptr_t unavSlots[numUnav];
	int yrow, xslot;
	for (int i = 0; i < numUnav; i++) {
		fscanf(input, "%d %d\n", &yrow, &xslot);
	}

	/* parse the server specs */
	server *srvl[numSrv];
	int size, power;
	for (int j = 0; j < numSrv; j++) {
		fscanf(input, "%d %d\n", &size, &power);

		server *srv = malloc(sizeof(server));
		srv->sz = size;
		srv->pw = power;
		srvl[j] = srv;
	}

	fclose(input);


	/* sort by pw/sz */
	qsort(&srvl[0], numSrv, sizeof(server *), sortsrvpps);

	/* sort by sz but only in blocks of 10 */
	int div = numSrv/10;
	int diff = numSrv - (div*10);
	for (int i = 0; i < div; i++) {
		qsort(&srvl[i*10], 10, sizeof(server *), sortsrvsz);
	}
	qsort(&srvl[div], diff, sizeof(server *), sortsrvsz);

}

/* sort server by power per size */
static int sortsrvpps(const void *srv1vp, const void *srv2vp)
{
	const server *srv1 = *(server * const *)srv1vp;
	const server *srv2 = *(server * const *)srv2vp;

	double pw1 = srv1->pw;
	double sz1 = srv1->sz;
	double pw2 = srv2->pw;
	double sz2 = srv2->sz;

	double pps1 = pw1/sz1;
	double pps2 = pw2/sz2;

	if (pps1 < pps2) {
		return 1;
	} else if (pps1 == pps2) {
		return 0;
	} else {
		return -1;
	}
}

/* sort servers by size */
static int sortsrvsz(const void *srv1vp, const void *srv2vp)
{
	const server *srv1 = *(server * const *)srv1vp;
	const server *srv2 = *(server * const *)srv2vp;

	int sz1 = srv1->sz;
	int sz2 = srv2->sz;

	if (sz1 < sz2) {
		return -1;
	} else if (sz1 == sz2) {
		return 0;
	} else {
		return 1;
	}
}

void printsrvlist(server *srvl[], int len)
{
	for (int j = 0; j < len; j++) {
		int sz = srvl[j]->sz;
		int pw = srvl[j]->pw;
		double pws = pw/sz;
		printf("sz: %d\tpw: %d\tpws: %f\n", sz, pw, pws);
	}
}
