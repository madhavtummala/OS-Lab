#include "MfsLibrary.h"
#include <stdio.h>

int main()
{
	filehandler fh;
	char *filename = "firstfile";

	fh = myfopen(filename);

	printf("File handler: %d\n", fh);
	return 0;
}