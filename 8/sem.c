#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

int buff[10];
int idx;
sem_t operate_sem;

void *reader(){

	sleep(1);
	while(1){
		while(sem_wait(&operate_sem)==-1){
			if(errno != EINTR) continue;
		}

		printf("Writer: ");
		scanf("%d", &buff[idx]);

		sem_post(&operate_sem);
	}

	return NULL;
}	

void *writer(){

	while(1){
		while(sem_wait(&operate_sem)==-1){
			if(errno != EINTR) continue;
		}

		printf("Reader: %d\n",buff[idx]);
		sem_post(&operate_sem);
	}

	return NULL;
}

int main(){

	if(sem_init(&operate_sem, 0, 1) == -1) 
		{printf("Failed\n");return -1;}

	pthread_t read, write;
	idx=0;
	buff[0]=1;

	pthread_create(&read, NULL, reader, NULL);
	pthread_create(&write, NULL, writer, NULL);

	pthread_join(read, NULL);
	pthread_join(write, NULL);

}