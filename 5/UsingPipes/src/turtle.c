#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int *tur_pos;
int *har_pos;

void signal_handler(int signum)
{
	if(signum == SIGUSR1){
		//printf("Came to signal\n");
		// char *ghar = "/tmp/ghar";
		char *gtur = "/tmp/gtur";

		mkfifo(gtur, 0666);
		// mkfifo(ghar, 0666);
		
		//printf("Ready to read in turtle\n");
		int fdgtur = open(gtur, O_RDONLY);
		read(fdgtur,tur_pos,sizeof(tur_pos));
		close(fdgtur);
		//printf("Recieved the value %d\n",*tur_pos);
		// scanf("%d", tur_pos);
	}
	// exit(signum);
}

int main()
{
	signal(SIGUSR1, signal_handler);
	tur_pos = malloc(sizeof(int *));
	har_pos = malloc(sizeof(int *));
	int turspeed=1;
	int fdtur,fdrtur,fdhar,time;

	char *har = "/tmp/har";
	char *tur = "/tmp/tur";
	char *rhar = "/tmp/rhar";
	char *rtur = "/tmp/rtur";

	mkfifo(tur, 0666);
	mkfifo(har, 0666);
	mkfifo(rtur, 0666);

	time=0;
	int finish=100;
	while(1)
	{
		sleep(1);
		time++;

		//printf("Reading from fdhar\n");
		fdhar = open(har, O_RDONLY);
		read(fdhar,har_pos,sizeof(har_pos));
		close(fdhar);
		//printf("Writing %d %d\n",*tur_pos,*har_pos);
		if(*har_pos >= finish) break; 

		//printf("Writing to fdtur\n");
		fdtur = open(tur, O_WRONLY);
		write(fdtur,tur_pos,sizeof(tur_pos));
		close(fdtur);
		//printf("Writing to fdrtur\n");
		fdrtur = open(rtur, O_WRONLY);
		write(fdrtur,tur_pos,sizeof(tur_pos));
		close(fdrtur);		
		if(*tur_pos >= finish) break;

		if(time>=turspeed){
			time=0;
			*tur_pos = *tur_pos + turspeed;
		}
		
	}
	exit(0);
}