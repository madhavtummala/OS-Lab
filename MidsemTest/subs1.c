#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

char *subs1 = "/tmp/subs1";

void signal_handler(int signum){
	if(signum == SIGALRM){
		printf("Recieved Alarm notification for subs1.\n");
	}
}

int main(){

	//signal(SIGALRM, signal_handler);

	mkfifo(subs1, 0666);
	int fd;
	int *pid = malloc(sizeof(int));

	*pid = getpid();

	fd = open(subs1, O_WRONLY);
	write(fd, pid, sizeof(pid));
	close(fd);

	while(1) signal(SIGALRM, signal_handler);

	return 0;
}