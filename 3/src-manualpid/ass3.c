#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>

int execute(char **args, int index, int pid)
{
    char **command = malloc(1024 * sizeof(char *));
    char str[80];
    strcpy(str,"./");
    strcat(str,args[index]);
    command[0]=str;
    command[1]=NULL;

    int fd;
    char buf[32];

    fd = open("/proc/sys/kernel/ns_last_pid", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("Can't open ns_last_pid");
        return 1;
    }

    if (flock(fd, LOCK_EX)) {
        close(fd);
        printf("Can't lock ns_last_pid\n");
        return 1;
    }

    snprintf(buf, sizeof(buf), "%d", pid - 1);

    if (write(fd, buf, strlen(buf)) != strlen(buf)) {
        printf("Can't write to buf\n");
        return 1;
    }

    int new_pid;
    new_pid = fork();
    if (new_pid == 0) {
        execvp(command[0],command);
        exit(0);
    } else if (new_pid == pid) {
        printf("PID for %s is %d\n",args[index],pid);
        waitpid(new_pid,0,0);
    } else {
        printf("Couldn't get the same PID\n");
        waitpid(new_pid,0,0);
    }

    if (flock(fd, LOCK_UN)) {
        printf("Can't unlock");
    }

    close(fd);

    return 0;
}

int main(int argc, char **argv)
{
    if(argc<2) return 0;

    char input_buffer[1024];
    fgets(input_buffer, 1024, stdin);
    char **args = malloc(1024 * sizeof(char *));

    int m=1,i,res;
    int pid = atoi(argv[1]);
    
    args[0]=strtok(input_buffer," ");       
    while((args[m]=strtok(NULL," "))!=NULL)
        m++;

    FILE* fp = fopen("temp.txt","w");
    fprintf(fp, "%s", args[m-1]);
    fclose(fp);

    for(i=0;i<m-1;i++)
        execute(args, i, pid);

    fp = fopen("temp.txt","r");
    fscanf(fp, "%d", &res);
    fclose(fp);   

    printf("%d\n",res);

    return 0;    
}