#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

char *rc1 = "/tmp/rc1";
char *wc1 = "/tmp/wc1";

#define BUFFSIZE 1024

void *ReadClient(){
	mkfifo(rc1, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdrc1;

	while(strcmp(buff,"q")!=0){
		printf("Client1: ");
		scanf("%[^\n]",buff);
		fdrc1=open(rc1, O_WRONLY);
		write(fdrc1,buff,sizeof(buff));
		close(fdrc1);
	}
	return NULL;
}

void *WriteClient(){
	mkfifo(wc1, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdwc1;

	while(strcmp(buff,"q")!=0){
		fdwc1=open(wc1, O_RDONLY);
		read(fdwc1,buff,sizeof(buff));
		close(fdwc1);
		printf("Server: %s\n",buff);
	}
	return NULL;
}

int main(){
	pthread_t c1r,c1w;

	pthread_create(&c1r, NULL, ReadClient, NULL);
	pthread_create(&c1w, NULL, WriteClient, NULL);

	pthread_join(c1r, NULL);
	pthread_join(c1w, NULL);

	return 0;
}