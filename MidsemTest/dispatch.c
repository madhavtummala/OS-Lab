/*

Implemented variable number of publishers and 3 subscribers

The state of program now allows us to set no of publishers to a max of 5
To create variable no of named pipes also instead of 5 and to have only one simple funciton
uncomment the commented lines and comment the current active part
*/



#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#define MAX 5

char *subs1 = "/tmp/subs1";
char *subs2 = "/tmp/subs2";
char *subs3 = "/tmp/subs3";

/* comment these lines and run the commented part of main
for true variable number of publishers without any limit.
and also comment all the extra functions except pub */
char *pubs1 = "/tmp/pubs1";
char *pubs2 = "/tmp/pubs2";
char *pubs3 = "/tmp/pubs3";
char *pubs4 = "/tmp/pubs4";
char *pubs5 = "/tmp/pubs5";

int *pid1;
int *pid2;
int *pid3;

int npubs;

void *pub1(){

	if(1>npubs) return NULL;

	mkfifo(pubs1, 0666);

	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		fd = open(pubs1, O_RDONLY);
		read(fd, message, sizeof(message));
		close(fd);

		if(*message==0) break;
		
		if(*message==1) kill(*pid1, SIGALRM);
		else if(*message==2) kill(*pid2, SIGALRM);
		else if(*message==3) kill(*pid3, SIGALRM);
	}

	return NULL;
}

void *pub2(){

	if(2>npubs) return NULL;

	mkfifo(pubs2, 0666);

	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		fd = open(pubs2, O_RDONLY);
		read(fd, message, sizeof(message));
		close(fd);

		if(*message==0) break;
		
		if(*message==1) kill(*pid1, SIGALRM);
		else if(*message==2) kill(*pid2, SIGALRM);
		else if(*message==3) kill(*pid3, SIGALRM);
	}

	return NULL;
}

void *pub3(){

	if(3>npubs) return NULL;

	mkfifo(pubs3, 0666);

	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		fd = open(pubs3, O_RDONLY);
		read(fd, message, sizeof(message));
		close(fd);

		if(*message==0) break;
		
		if(*message==1) kill(*pid1, SIGALRM);
		else if(*message==2) kill(*pid2, SIGALRM);
		else if(*message==3) kill(*pid3, SIGALRM);
	}

	return NULL;
}

void *pub4(){

	if(4>npubs) return NULL;

	mkfifo(pubs4, 0666);

	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		fd = open(pubs4, O_RDONLY);
		read(fd, message, sizeof(message));
		close(fd);

		if(*message==0) break;
		
		if(*message==1) kill(*pid1, SIGALRM);
		else if(*message==2) kill(*pid2, SIGALRM);
		else if(*message==3) kill(*pid3, SIGALRM);
	}

	return NULL;
}

void *pub5(){

	if(5>npubs) return NULL;

	mkfifo(pubs5, 0666);

	int fd;
	int *message = malloc(sizeof(int));

	while(1){
		fd = open(pubs5, O_RDONLY);
		read(fd, message, sizeof(message));
		close(fd);

		if(*message==0) break;
		
		if(*message==1) kill(*pid1, SIGALRM);
		else if(*message==2) kill(*pid2, SIGALRM);
		else if(*message==3) kill(*pid3, SIGALRM);
	}

	return NULL;
}

// void *pub(char *s){

// 	mkfifo(s, 0666);

// 	int fd;
// 	int *message = malloc(sizeof(int));

// 	while(1){
// 		fd = open(s, O_RDONLY);
// 		read(fd, message, sizeof(message));
// 		close(fd);

// 		if(*message==0) break;
		
// 		if(*message==1) kill(*pid1, SIGALRM);
// 		else if(*message==2) kill(*pid2, SIGALRM);
// 		else if(*message==3) kill(*pid3, SIGALRM);
// 	}

// 	 return NULL;	
// }

void *sub1(){
	int fd;

	fd = open(subs1, O_RDONLY);
	read(fd, pid1, sizeof(pid1));
	close(fd);

	printf("Registered subs1 pid = %d\n",*pid1);
	return NULL;
}

void *sub2(){
	int fd;
	
	fd = open(subs2, O_RDONLY);
	read(fd, pid2, sizeof(pid1));
	close(fd);

	printf("Registered subs2 pid = %d\n",*pid1);
	return NULL;
}

void *sub3(){
	int fd;
	
	fd = open(subs3, O_RDONLY);
	read(fd, pid3, sizeof(pid1));
	close(fd);

	printf("Registered subs3 pid = %d\n",*pid1);
	return NULL;
}

int main(){

	mkfifo(subs1, 0666);
	mkfifo(subs2, 0666);
	mkfifo(subs3, 0666);

	pid1 = malloc(sizeof(int));
	pid2 = malloc(sizeof(int));
	pid3 = malloc(sizeof(int));

	int fd,i;

	printf("Enter the number of publishers\n");
	scanf(" %d",&npubs);

	printf("All set\n");

	// fd = open(subs1, O_RDONLY);
	// read(fd, pid1, sizeof(pid1));
	// close(fd);

	// printf("Registered subs1 pid = %d\n",*pid1);

	// fd = open(subs2, O_RDONLY);
	// read(fd, pid2, sizeof(pid2));
	// close(fd);

	// printf("Registered subs2 pid = %d\n",*pid2);

	// fd = open(subs3, O_RDONLY);
	// read(fd, pid3, sizeof(pid3));
	// close(fd);

	// printf("Registered subs3 pid = %d\n",*pid3);

	pthread_t s1,s2,s3;
	pthread_create(&s1, NULL, sub1, NULL);
	pthread_create(&s2, NULL, sub2, NULL);
	pthread_create(&s3, NULL, sub3, NULL);

	pthread_join(s1, NULL);
	pthread_join(s2, NULL);
	pthread_join(s3, NULL);

	// pthread_t pubs[npubs];
	// char *s = "/tmp/pubsi";

	// for(i=1;i<=npubs;i++){
	// 	s[9]='0'+i;
	// 	pthread_create(&pubs[i], NULL, pub, s);
	// }

	// for(i=0;i<npubs;i++){
	// 	pthread_join(pubs[i], NULL);
	// }

	pthread_t p1,p2,p3,p4,p5;

	pthread_create(&p1, NULL, pub1, NULL);
	pthread_create(&p2, NULL, pub2, NULL);
	pthread_create(&p3, NULL, pub3, NULL);
	pthread_create(&p4, NULL, pub4, NULL);
	pthread_create(&p5, NULL, pub5, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);
	pthread_join(p5, NULL);

	return 0;

}