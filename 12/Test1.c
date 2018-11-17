#include "MfsLibrary.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	filehandler fh;
	char *filename = "firstfile";

	fh = myfopen(filename);

	printf("File handler: %d\n", fh);

	char *out = (char*)malloc(sizeof(char)*50);

	myfread(out, 50, fh);
	return 0;
}