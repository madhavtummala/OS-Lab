#include "MfsLibrary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

int main()
{
	filehandler fh;
	char *out1 = (char*)malloc(sizeof(char)*40);
	char *out2 = (char*)malloc(sizeof(char)*40);
	char *str;

	char *filename = "firstfil";
	fh = myfopen(filename);

	str = "short";

	myfwrite(str, strlen(str), fh);
	myfread(out1, 40, fh);

	printf("Out: %s\n",out1);
	myfclose(fh);

	fh = myfopen("secondfi");

	str = "this is longer than before for second file";

	myfwrite(str, strlen(str), fh);
	myfread(out2, 40, fh);

	printf("Out: %s\n",out2);
	myfclose(fh);

	return 0;
}