#include <stdio.h>
#include <unistd.h>

#define MAX_ITR 100000

int main()
{
	int i,j,pid;

	pid = fork();

	if(pid==0){
		//c
		for(i=0;i<20;i++){
			sleep(1);
			printf("Child: %d\n",i);
		}
	}else{
		//p
		for(i=0;i<20;i++){
			sleep(1);
			printf("Parent: %d\n",i);
		}
	}

	return 0;
}