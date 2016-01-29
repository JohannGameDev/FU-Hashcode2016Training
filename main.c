#include "main.h"


int main()
{
	FILE *input = fopen("dc.in", "ro");

	/* parse file */
	unsigned int numRows, slotsPRow, numUnav, numPools, numSrv;
	fscanf(input, "%u %u %u %u %u\n", &numRows, &slotsPRow, &numUnav, &numPools, &numSrv);

	printf( "== INPUT ==\n"
		"number of rows:\t\t%d\n"
		"slots per row:\t\t%d\n"
		"number of unav servers:\t%d\n"
		"number of pools:\t%d\n"
		"number of servers:\t%d\n"
		"== END ==\n",
		numRows, slotsPRow, numUnav, numPools, numSrv);

	/* parse unavailable slots */
	coord *unavSlots[numUnav];
	unsigned int yrow, xslot;
	for (int i = 0; i < numUnav; i++) {
		fscanf(input, "%u %u\n", &yrow, &xslot);

		coord *dfc = malloc(sizeof(coord));
		dfc->row = yrow;
		dfc->slot = xslot;
		unavSlots[i] = dfc;
	}

	/* parse the server specs */
	server *srvl[numSrv];
	unsigned int size, power;
	for (unsigned int i = 0; i < numSrv; i++) {
		fscanf(input, "%u %u\n", &size, &power);

		server *srv = malloc(sizeof(server));
		srv->sz = size;
		srv->pw = power;
		srv->id = i;
		srvl[i] = srv;
	}

	fclose(input);


	/* sort by pw/sz */
	qsort(&srvl[0], numSrv, sizeof(server *), sortsrvpps);

	/* sort by sz but only in blocks of 10 */
	unsigned int div = numSrv/10;
	unsigned int diff = numSrv - (div*10);
	for (int i = 0; i < div; i++) {
		qsort(&srvl[i*10], 10, sizeof(server *), sortsrvsz);
	}
	qsort(&srvl[div], diff, sizeof(server *), sortsrvsz);


	/* insert servers into rows */
	char rows[numRows][slotsPRow];

	/* init rows */
	for (int i = 0; i < numRows; i++) {
		memset(&rows[i][0], 0, slotsPRow);
	}

	printf("\n");


	for (int i = 0; i < numUnav; i++) {
		rows[unavSlots[i]->row][unavSlots[i]->slot] = 'O';
	}

	printrows(slotsPRow, numRows, rows);

	/* take each server. for each, try fitting it in the first row, if this
	*  doesn't work, try the next row etc.
	*  if all rows are occupied or otherwise insertion fails, try the next
	*  server.
	*/
	unsigned int i = 0;
	unsigned int j = 0;
	/* indicates whether we've reset the rows. if we've done that and run
	*  through all rows but still have 'clean' rows, we cannot fit anything
	*  anymore
	*/
	int clean = 0;
	while (i < numSrv) {
		printf("TRY SERVER %d/%d\n", i, numSrv);
		if (j >= numRows) {
			if (clean) {
				i++;
			}
			j = 0;
			clean = 1;
		}

		printf("(I) processing srv %d (sz, pw): %u, %u\n", i, srvl[i]->sz, srvl[j]->pw);
		printrow(slotsPRow, rows[j]);
		if (tryInsert(srvl[i], rows[j], slotsPRow)) {
			srvl[i]->co.row = j;
			printrow(slotsPRow, rows[j]);
			printf("(S) inserted\n");
			i++;
			j++;
			continue;
		}
		printrow(slotsPRow, rows[j]);
		printf("(F) no slot here\n");
		j++;
	}

	printf("== RESULT ==\n");
	printrows(slotsPRow, numRows, rows);

	qsort(&srvl[0], numSrv, sizeof(server *), sortsrvid);
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

	unsigned int sz1 = srv1->sz;
	unsigned int sz2 = srv2->sz;

	if (sz1 < sz2) {
		return -1;
	} else if (sz1 == sz2) {
		return 0;
	} else {
		return 1;
	}
}

static int sortsrvid(const void *srv1vp, const void *srv2vp)
{
	const server *srv1 = *(server * const *)srv1vp;
	const server *srv2 = *(server * const *)srv2vp;

	unsigned int id1 = srv1->id;
	unsigned int id2 = srv2->id;

	if (id1 < id2) {
		return -1;
	} else if (id1 == id2) {
		return 0;
	} else {
		return 1;
	}
}

void printresultlist(server *srvl[], int len)
{
	FILE *result = fopen("result.out", "rw");
	for (int j = 0; j < len; j++)
	{
		/* TODO: replace srvl[j]->id with pool assignment */
		fprintf(result, "%u %u %u", srvl[j]->co.row, srvl[j]->co.slot, srvl[j]->id);
	}
}

void printsrvlist(server *srvl[], int len)
{
	for (int j = 0; j < len; j++) {
		unsigned int sz = srvl[j]->sz;
		unsigned int pw = srvl[j]->pw;
		double pws = pw/sz;
		printf("sz: %d\tpw: %d\tpws: %f\n", sz, pw, pws);
	}
}

/* try finding a slot in the row, mark all slots now occupied and write server
 * coordinates.
 *
 * return 1 if successful
 */
int tryInsert(server *srv, char *row, unsigned int rlen)
{
	for (unsigned int i = 0; i < rlen; i++) {
		/* inv: slots occupied ? */
		int inv = 0;
		for (unsigned int j = 0; j < srv->sz; j++) {
			if (row[i+j]) {
				inv = 1;
				break;
			}
		}

		if (inv) continue;

		/* row is filled in by caller */
		srv->co.slot = i;
		/* all slots are free, put server there */
		for (unsigned int j = 0; j < srv->sz; j++) {
			row[i+j] = 'x';
		}
		return 1;
	}

	return 0;
}

void printrow(const unsigned int rlen, char rows[rlen])
{
	printf("[");
	for (int j = 0; j < rlen; j++) {
		if (rows[j] == '\0') {
			printf(" ");
		} else {
			printf("%c", rows[j]);
		}
	}
	printf("]\n");
}

void printrows(const unsigned int rlen, const unsigned int num, char rows[][rlen])
{
	for (int i = 0; i < num; i++)
	{
		printrow(rlen, rows[i]);
	}
}


