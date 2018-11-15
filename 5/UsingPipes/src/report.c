#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define clear() printf("\033[H\033[J")

int main()
{
	int *status;
	int fdrtur,fdrhar;
	int finish=100;
	int i;

	char *har = "/tmp/har";
	char *tur = "/tmp/tur";
	char *rhar = "/tmp/rhar";
	char *rtur = "/tmp/rtur";
	
	mkfifo(rtur,0666);
	mkfifo(rhar,0666);

	int *tur_pos = malloc(sizeof(int *));
	int *har_pos = malloc(sizeof(int *));
	while(1)
	{
		clear();
		fdrhar = open(rhar, O_RDONLY);
		read(fdrhar,har_pos,sizeof(har_pos));
		close(fdrhar);
		if(*har_pos >= finish){printf("\nHare Won\n");break;}

		fdrtur = open(rtur, O_RDONLY);
		read(fdrtur,tur_pos,sizeof(har_pos));
		close(fdrtur);		
		if(*tur_pos >= finish){printf("\nTurtle Won\n");break;}

		for(i=0;i<100;i++){
			if(i==(*har_pos)) printf("@");
			else printf(" ");
		}
		printf("\n");
		for(i=0;i<100;i++){
			if(i==(*tur_pos)) printf("$");
			else printf(" ");
		}
		printf("\n");
	}

	exit(0);
	
}