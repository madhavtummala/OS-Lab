#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

char *pubs3 = "/tmp/pubs3";

int main(){

	mkfifo(pubs3, 0666);
	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		
		scanf(" %d", message);

		fd = open(pubs3, O_WRONLY);
		write(fd, message, sizeof(message));
		close(fd);
	}

}