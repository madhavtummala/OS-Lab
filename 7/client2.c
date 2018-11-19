#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

char *rc2 = "/tmp/rc2";
char *wc2 = "/tmp/wc2";

#define BUFFSIZE 1024

void *ReadClient(){
	mkfifo(rc2, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdrc2;

	while(strcmp(buff,"q")!=0){
		printf("Client1: ");
		scanf("%[^\n]",buff);
		fdrc2=open(rc2, O_WRONLY);
		write(fdrc2,buff,sizeof(buff));
		close(fdrc2);
	}
	return NULL;
}

void *WriteClient(){
	mkfifo(wc2, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdwc2;

	while(strcmp(buff,"q")!=0){
		fdwc2=open(wc2, O_RDONLY);
		read(fdwc2,buff,sizeof(buff));
		close(fdwc2);
		printf("Server: %s\n",buff);
	}
	return NULL;
}

int main(){
	pthread_t c2r,c2w;

	pthread_create(&c2r, NULL, ReadClient, NULL);
	pthread_create(&c2w, NULL, WriteClient, NULL);

	pthread_join(c2r, NULL);
	pthread_join(c2w, NULL);

	return 0;
}