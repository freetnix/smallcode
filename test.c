#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	int L = 10, R = 15;

	int K = 0, i = 5;

	while (i-- > 0) {
		K = (int)(rand()%(R-L+1));
		printf("K=[%d]\n", K);
	}

	return 0;
}
