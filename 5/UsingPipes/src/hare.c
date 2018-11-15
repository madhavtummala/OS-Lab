#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

int *tur_pos;
int *har_pos;

void signal_handler(int signum)
{
	if(signum == SIGUSR1){
		//printf("Came to signal\n");
		// scanf("%d",har_pos);
		char *ghar = "/tmp/ghar";
		// char *gtur = "/tmp/gtur";

		// mkfifo(gtur, 0666);
		mkfifo(ghar, 0666);

		//printf("Ready to read in hare\n");
		int fdghar = open(ghar, O_RDONLY);
		read(fdghar,har_pos,sizeof(har_pos));
		close(fdghar);
		//printf("Recieved Value %d\n",*har_pos);
	}
	// exit(signum);
}

int main()
{
	signal(SIGUSR1, signal_handler);
	tur_pos = malloc(sizeof(int *));
	har_pos = malloc(sizeof(int *));
	int harspeed=5,harsleep=2;
	int finish=100;
	int fdtur,fdhar,fdrhar,time;
	int sep=harspeed*2;

	char *har = "/tmp/har";
	char *tur = "/tmp/tur";
	char *rhar = "/tmp/rhar";
	char *rtur = "/tmp/rtur";

	mkfifo(tur, 0666);
	mkfifo(har, 0666);
	mkfifo(rhar, 0666);

	time=0;
	while(1)
	{
		sleep(1);
		//printf("Writing to fdhar\n");
		fdhar = open(har, O_WRONLY);
		write(fdhar,har_pos,sizeof(har_pos));
		close(fdhar);
		//printf("Writing to fdhar\n");
		fdrhar = open(rhar, O_WRONLY);
		write(fdrhar,har_pos,sizeof(har_pos));
		close(fdrhar);		
		if(*har_pos >= finish) break;

		//printf("Writing to fdtur\n");
		fdtur = open(tur, O_RDONLY);
		read(fdtur,tur_pos,sizeof(tur_pos));
		close(fdtur);
		if(*tur_pos >= finish) break;

		if(time!=0){
			time++;
			if(time>harsleep)
				time=0;
		}

		if(*tur_pos > *har_pos - sep){
			if(time==0)
				*har_pos = *har_pos + harspeed;
		}
		else if(time==0)
			time++;

		//printf("hare:%d turtle:%d time:%d\n", *har_pos, *tur_pos, time);
	}

	exit(0);
}