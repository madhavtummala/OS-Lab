#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signum){
	if(signum == SIGINT){
		printf("CTRL+C is pressed\n");
	}
	else if (signum == SIGTSTP){
		printf("CTRL+Z is pressed\n");
	}
	else if(signum == SIGALRM){
		printf("Alarm\n");
	}
}

int main()
{
	signal(SIGTSTP, signal_handler);
	signal(SIGALRM, signal_handler);
	signal(SIGINT, signal_handler);

	while(1){
		sleep(2);
		kill(getpid(),SIGALRM);
	}

	return 0;
}