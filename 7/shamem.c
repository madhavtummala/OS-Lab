#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void writer(){
	printf("In writer now\n");
	key_t key = ftok("shmfile", 65);
	int shmid = shmget(key,1024, 0666|IPC_CREAT);
	char *str = (char *) shmat(shmid, (void *)0, 0);
	//str="init";
	scanf(" %s",str);
	shmdt(str);
}

void reader(){
	sleep(2);
	printf("In reader now\n");
	key_t key = ftok("shmfile", 65);
	int shmid = shmget(key,1024, 0666|IPC_CREAT);
	char *str = (char *) shmat(shmid, (void *)0, 0);
	printf("In reader %s\n", str);
	shmdt(str);
	shmctl(shmid,IPC_RMID,NULL); 
}

int main(){
	if(fork()==0){
		writer();
	}
	else{
		reader();
		wait(NULL);
	}
	return 0;
}