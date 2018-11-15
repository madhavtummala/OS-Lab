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
char *rc2 = "/tmp/rc2";
char *wc2 = "/tmp/wc2";

#define BUFFSIZE 1024

bool connect1,connect2;

void *ReadClient1(){
	mkfifo(rc1, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdrc1;

	while(strcmp(buff,"q")!=0){
		fdrc1=open(rc1, O_RDONLY);
		read(fdrc1,buff,sizeof(buff));
		close(fdrc1);
		printf("Client1: %s\n",buff);
	}
	connect1=false;
	printf("Connection 1 closed\n");
	return NULL;
}

void *ReadClient2(){
	mkfifo(rc2, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdrc2;

	while(strcmp(buff,"q")!=0){
		fdrc2=open(rc2, O_RDONLY);
		read(fdrc2,buff,sizeof(buff));
		close(fdrc2);
		printf("Client2: %s\n",buff);
	}
	connect2=false;
	printf("Connection 2 closed\n");
	return NULL;
}

void *WriteClients(){
	mkfifo(wc1, 0666);
	mkfifo(wc2, 0666);
	char *buff = malloc(BUFFSIZE * sizeof(char));
	int fdwc1,fdwc2;

	while(1){
		printf("Server: ");
		scanf("%[^\n]",buff);
		if(connect1){
			fdwc1=open(wc1, O_WRONLY);
			write(fdwc1,buff,sizeof(buff));
			close(fdwc1);
		}
		if(connect2){
			fdwc2=open(wc2, O_WRONLY);
			write(fdwc2,buff,sizeof(buff));	
			close(fdwc2);		
		}
		if(!connect1 && !connect2) break;
	}

	printf("Both connections closed, quitting ...\n");
	return NULL;
}

int main(){
	pthread_t c1r,c2r,cw;
	connect1=true;
	connect2=true;

	pthread_create(&c1r, NULL, ReadClient1, NULL);
	pthread_create(&c2r, NULL, ReadClient2, NULL);
	pthread_create(&cw, NULL, WriteClients, NULL);

	pthread_join(c1r, NULL);
	pthread_join(c2r, NULL);
	pthread_join(cw, NULL);

	return 0;
}