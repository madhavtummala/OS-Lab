#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_SIZ 80


int main()
{
	FILE *fp;
	char str[LINE_SIZ];

	fp = fopen("plain.txt", "r");
	if(fp==NULL){
		fprintf(stderr, "File not found\n");
		exit(1);
	}

	fgets(str,LINE_SIZ,fp);
	printf("Parent reads: %s\n",str);

	if(fork()==0){
		fgets(str, LINE_SIZ, fp);
		printf("Child reads: %s\n",str);
	}

	fclose(fp);
}