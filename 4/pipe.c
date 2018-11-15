#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CMDBUFSIZE 1024

char **args;

int mad_execute(int index, int input, int n)
{
	int p[2], i;

	if(pipe(p)<0){
		printf("pipe failed\n");
		exit(0);
	}

	int f = fork();

	if(f<0){
		printf("forked failed\n");
		exit(0);
	}

	else if(f==0)
	{
		//child process

		if(index==0 && n==1)
			dup2(p[1],1);

		else if(index==n)
			dup2(input,0);

		else{
			dup2(input,0);
			dup2(p[1],1);
		}

		if(execvp(args[0],args)<0){
			printf("Program failed to execute\n");
			exit(0);
		}

	}
	else
	{
		//parent process
		waitpid(f, 0, 0);
	}

	close(p[1]);
	if(index!=0) close(input);
	if(index==n) close(p[0]);

	return p[0];
}

void mad_parse(char *input_buffer)
{
	int i,n=1,m,input=0;

	args = malloc(CMDBUFSIZE * sizeof(char *));
	char **cmd = malloc(CMDBUFSIZE * sizeof(char *));

  cmd[0]=strtok(input_buffer,"|");
  while ((cmd[n]=strtok(NULL,"|"))!=NULL)
      n++;
  cmd[n]=NULL;

  for(i=0;i<n;i++)
  {
  	m=1;
    args[0]=strtok(cmd[i]," ");
    while((args[m]=strtok(NULL," "))!=NULL)
      m++;
    args[m]=NULL;

    if(args[0]==NULL) continue;

    input = mad_execute(i,input,n-1); 	
  }

}

int main()
{
  char input_buffer[CMDBUFSIZE];
  char ch[CMDBUFSIZE] = "\n";

  fgets(input_buffer, CMDBUFSIZE, stdin);

  if(strcmp(input_buffer,ch)==0) return 0;
  int len = strlen(input_buffer);
  input_buffer[len-1]='\0';

  mad_parse(input_buffer);

  return 0;

}	