#include <sys/wait.h> //for waitpid()
#include <unistd.h>  //for chdir(), fork(), exec(), pid_t
#include <stdlib.h> //for malloc(), realloc(), free(), exit(), execvp(), EXIT_FAILURE, EXIT_SUCESSS
#include <stdio.h> //for stderr, getchar(), perror(), printf(), scanf()
#include <string.h> //for strcmp(), strtok()
#include <sys/types.h> //for pid, exec() stuff

#include <stdlib.h>

int main(int argc, char **argv)
{
    char **command = malloc(1024 * sizeof(char *));
    int i;

    for(int i=1;i<argc;i++)
        command[i-1] = argv[i];
    command[i-1]=NULL;

    int a = atoi(command[argc-2]);

    sprintf(command[argc-2], "%d", a*a);

    if(command[1]==NULL){
    	printf("%s\n",command[argc-2]);
    	exit(0);
    }

    char str[80];
    strcpy(str,"./");
    strcat(str,command[0]);
    command[0] = str;

    printf("PID of %s: %d\n",command[0],getpid());
    if(execvp(command[0],command)<0){
        printf("Command failed\n");
        exit(0);
    }

    return 0;
}