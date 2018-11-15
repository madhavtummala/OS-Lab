#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MAX 10

char *sharedfile = "/tmp/shafile";
//sem_t mutex1;
//sem_t mutex2;
//sem_t mutex3;

int main(){

	mkfifo(sharedfile,0666);
	//sem_init(&mutex1, 1, 0);
	//sem_init(&mutex2, 1, 0);
	//sem_init(&mutex3, 1, 0);

	if(fork()==0){
		//child 1
		int *var = malloc(sizeof(int));
		*var = 0;
		int fd;
		printf("Came to child PID %d\n", getpid());
		while(*var<MAX){
			//sem_wait(&mutex2);
			sleep(3);
			fd=open(sharedfile, O_RDONLY);
			read(fd, var, sizeof(var));
			close(fd);
			printf("PID:%d took %d\n", getpid(),*var);
			//sem_post(&mutex3);
		}
		exit(0);
	}

	else{
		if(fork()==0){
			//child 2
			//doing nothing for now
			int *var = malloc(sizeof(int));
			*var = 0;
			int fd;
			printf("Came to child PID %d\n", getpid());
			while(*var<MAX){
				//sem_wait(&mutex3);
				sleep(2);
				fd=open(sharedfile, O_RDONLY);
				read(fd, var, sizeof(var));
				close(fd);
				printf("PID:%d took %d\n", getpid(),*var);
				//sem_post(&mutex1);
			}
			exit(0);			
		}
		else{
			//parent
			int *var = malloc(sizeof(int));
			*var = 0;
			int fd;
			printf("Came to parent PID %d\n", getpid());
			//printf("%d\n",mutex1);
			//sem_post(&mutex1);
			//printf("%d\n",mutex1);
			while(*var<MAX){
				// scanf(" %d", var);
				//sem_wait(&mutex1);
				sleep(1);
				*var = *var + 1;
				fd=open(sharedfile, O_WRONLY);
				write(fd, var, sizeof(var));
				close(fd);
				printf("PID:%d gave %d\n",getpid(),*var);
				//sem_post(&mutex2);
			} 
			wait(NULL);
			exit(0);
		}
	}

	//sem_destroy(&mutex1);
	//sem_destroy(&mutex2);
	//sem_destroy(&mutex3);
}
