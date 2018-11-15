#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX 10

int main(){

	char *shafile = "/tmp/shafile";
	mkfifo(shafile, 0666);

	int *var = malloc(sizeof(int));
	*var = 0;
	int fd;
	while(*var<MAX){
		fd = open(shafile, O_WRONLY);
		write(fd,var,sizeof(var));
		close(fd);
		printf("Written %d to fifo\n",*var);
		// int i;
		// for(i=0;i<10;i++) printf("Waiting for it to go to read part\n");
		fd = open(shafile, O_RDONLY);
		printf("file openning : %d\n",fd);
		read(fd,var,sizeof(var));
		close(fd);
		printf("Read %d from fifo\n",*var);
		*var = *var+1;
	}
	return 0;
}