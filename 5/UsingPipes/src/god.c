#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int rpid,hpid,tpid;
int *tur_pos;
int *har_pos;

void signal_handler(int signum)
{
	if(signum == SIGUSR1){
		printf("Came to signal_handler\n");
		//printf("PID: %d %d %d\n",rpid,hpid,tpid);
		int fdghar,fdgtur;
		// kill(rpid, SIGINT);
		kill(tpid, SIGUSR1);
		// sleep(1);		
		kill(hpid, SIGUSR1);

		char *ghar = "/tmp/ghar";
		char *gtur = "/tmp/gtur";
		char ch;

		mkfifo(gtur,0666);
		mkfifo(ghar,0666);

		while(1){
			printf("Enter input\n");
			scanf(" %c",&ch);
			if(ch=='h')
				scanf("%d",har_pos);
			else if(ch=='t')
				scanf("%d",tur_pos);
			else if(ch=='q')
				break;
			//printf("%d %d\n",*har_pos,*tur_pos);
		}

		//printf("Writing tur\n");
		fdgtur = open(gtur, O_WRONLY);
		write(fdgtur,tur_pos,sizeof(tur_pos));
		close(fdgtur);

		//printf("Writing har\n");
		fdghar = open(ghar, O_WRONLY);
		write(fdghar,har_pos,sizeof(har_pos));
		close(fdghar);

		printf("completed signal_handler\n");
	}
}

int main()
{
	signal(SIGUSR1, signal_handler);
	int *status;
	int fdrtur,fdrhar;
	int finish=100;

	char *har = "/tmp/har";
	char *tur = "/tmp/tur";
	char *rhar = "/tmp/rhar";
	char *rtur = "/tmp/rtur";
	
	mkfifo(rtur,0666);
	mkfifo(rhar,0666);

	tur_pos = malloc(sizeof(int *));
	har_pos = malloc(sizeof(int *));

	hpid=fork();

	if(hpid==0){
		//run har
		hpid = getpid();
		char *args[] = {"./hare", NULL};
		if(execvp(args[0],args)<0)
			printf("Command cannot be executed\n");
	}
	else{
		tpid = fork();
		if(tpid==0){
			//run tur
			tpid = getpid();
			char *args[] = {"./tur", NULL};
			if(execvp(args[0],args)<0)
				printf("Command cannot be executed\n");
		}
		else{
			rpid = fork();
			if(rpid==0){
				//run report
				rpid = getpid();
				char *args[] = {"./report", NULL};
				if(execvp(args[0],args)<0)
					printf("Command cannot be executed\n");				
			}
			else{
				waitpid(rpid,0,0);
				waitpid(hpid,0,0);
				waitpid(tpid,0,0);
				// kill(hpid, SIGINT);
				// kill(tpid, SIGINT);
				exit(0);
			}
		}
	}
}