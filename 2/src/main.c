#include <sys/wait.h> //for waitpid()
#include <unistd.h>  //for chdir(), fork(), exec(), pid_t
#include <stdlib.h> //for malloc(), realloc(), free(), exit(), execvp(), EXIT_FAILURE, EXIT_SUCESSS
#include <stdio.h> //for stderr, getchar(), perror(), printf(), scanf()
#include <string.h> //for strcmp(), strtok()
#include <sys/types.h> //for pid, exec() stuff

#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h> //for mkdir
#include <errno.h> //for error handling
#include <dirent.h> //for dir things like ls -l
#include <signal.h> //for kill

#define clear() printf("\033[H\033[J")

void init_shell()
{
    clear();
    printf("\n\n\n\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("  ************   MAD SHELL   **************\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("\n\n\n");
    // sleep(1);
}

/*GLOBAL VARIABLES*/
int pipe_count=0, fd;
static char* args[512];
char *history_file;
char input_buffer[1024];
char *cmd_exec[100];
int flag, len;
char cwd[1024];

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}

char current_directory[1000];
int output_redirection, input_redirection;
char ret_file[3000];
char his_var[2000];
char *input_redirection_file;
char *output_redirection_file;
extern char** environ;
int background;

void clear_variables()
{
    fd =0;
    flag=0;
    len=0;
    pipe_count=0;
    output_redirection=0;
    input_redirection=0;
    input_buffer[0]='\0';
    cwd[0] = '\0';
    background=0;
}

void parent_directory()
{
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd );
    }
    else perror("getcwd() error");
}

static char* skipwhite(char* s)
{
    while (isspace(*s)) ++s;
    return s;
}
void mad_parse_commands(char *com_exec)
{
    int m=1;
    args[0]=strtok(com_exec," ");
    while((args[m]=strtok(NULL," "))!=NULL)
        m++;
}
void mad_parse_i_o(char *cmd_exec)
{
    char *io_token[100];
    char *new_cmd_exec1;
    new_cmd_exec1=strdup(cmd_exec);
    int m=1;
    io_token[0]=strtok(new_cmd_exec1,"<");
    while((io_token[m]=strtok(NULL,">"))!=NULL)
        m++;
    io_token[1]=skipwhite(io_token[1]);
    io_token[2]=skipwhite(io_token[2]);
    input_redirection_file=strdup(io_token[1]);
    output_redirection_file=strdup(io_token[2]);
    mad_parse_commands(io_token[0]);

}
void mad_parse_i(char *cmd_exec)
{
    char *i_token[100];
    char *new_cmd_exec1;
    new_cmd_exec1=strdup(cmd_exec);
    int m=1;
    i_token[0]=strtok(new_cmd_exec1,"<");
    while((i_token[m]=strtok(NULL,"<"))!=NULL)
        m++;
    i_token[1]=skipwhite(i_token[1]);
    input_redirection_file=strdup(i_token[1]);
    mad_parse_commands(i_token[0]);
}
void mad_parse_o(char *cmd_exec)
{
    char *o_token[100];
    char *new_cmd_exec1;
    new_cmd_exec1=strdup(cmd_exec);
    int m=1;
    o_token[0]=strtok(new_cmd_exec1,">");
    while((o_token[m]=strtok(NULL,">"))!=NULL)
        m++;
    o_token[1]=skipwhite(o_token[1]);
    output_redirection_file=strdup(o_token[1]);
    mad_parse_commands(o_token[0]);

}
char* skipcomma(char* str)
{
    int i=0, j=0;
    char temp[1000];
    while(str[i++]!='\0')
    {
        if(str[i-1]!='"')
            temp[j++]=str[i-1];
    }
    temp[j]='\0';
    str = strdup(temp);

    return str;
}

static int mad_execute(int input, int first, int last, char *cmd_exec)
{

    int mypipefd[2], ret, input_fd, output_fd;
    ret = pipe(mypipefd);
    if(ret == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        if (first==1 && last==0 && input==0) {
            dup2( mypipefd[1], 1 );
        }
        else if (first==0 && last==0 && input!=0)
        {
            dup2(input, 0);
            dup2(mypipefd[1], 1);
        }
        else {
            dup2(input, 0);
        }
        if (strchr(cmd_exec, '<') && strchr(cmd_exec, '>'))
        {
            input_redirection=1;
            output_redirection=1;
            mad_parse_i_o(cmd_exec);
        }
        else if (strchr(cmd_exec, '<'))
        {
            input_redirection=1;
            mad_parse_i(cmd_exec);
        }
        else if (strchr(cmd_exec, '>'))
        {
            output_redirection=1;
            mad_parse_o(cmd_exec);
        }
        if(output_redirection == 1) {
            output_fd= creat(output_redirection_file, 0644);
            if (output_fd < 0) {
                fprintf(stderr, "Failed to open %s for writing\n", output_redirection_file);
                return(EXIT_FAILURE);
            }
            dup2(output_fd, 1);
            close(output_fd);
            output_redirection=0;
        }
        if(input_redirection  == 1) {
            input_fd=open(input_redirection_file,O_RDONLY, 0);
            if (input_fd < 0) {
                fprintf(stderr, "Failed to open %s for reading\n", input_redirection_file);
                return(EXIT_FAILURE);
            }
            dup2(input_fd, 0);
            close(input_fd);
            input_redirection=0;
        }
        else if(execvp(args[0], args)<0) {
            //General category for exe files
            printf("Executing file: %s\n",args[0]);
            char str[80];
            strcpy(str, "./");
            strcat(str,args[0]);
            args[0]=str;
            // printf("Executing file: %s\n",args[0]);
            if(execvp(args[0], args)<0) {
                printf("Executable not found\n");
                exit(0);
            }
        }
    }
    else if(background==0) {
        waitpid(pid, 0, 0);
    }

    if (last == 1)
        close(mypipefd[0]);
    if (input != 0)
        close(input);
    close(mypipefd[1]);

    return mypipefd[0];
}

int mad_cd(char **args);
int mad_help(char **args);
int mad_exit(char **args);
int mad_pwd(char **args);
int mad_env(char **args);
int mad_mkdir(char **args);
int mad_rmdir(char **args);
int mad_ls(char **args);
int mad_cp(char **args);

extern int errno; //to handle errors
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "env",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &mad_cd,
    &mad_help,
    &mad_env,
    &mad_exit
};

int mad_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int mad_cd(char **args)
{
    // printf("Came here\n");
    char *h="/home";
    if(args[1]==NULL)
        chdir(h);
    else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
        chdir(h);
    else if(chdir(args[1])<0)
        printf("bash: cd: %s: No such file or directory\n", args[1]);

    return 1;
}

int mad_pwd(char **args)
{
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd );
    }
    else perror("getcwd() error");

    return 1;
}

int mad_mkdir(char **args)
{
    if(args[1]==NULL) {
        printf("Enter location\n");
        return 0;
    }
    char *str = cwd;
    strcat(str, args[1]);
    if(mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1)
        perror("Mad_shell: ");

    return 1;
}

int mad_rmdir(char **args)
{
    if(args[1]==NULL) {
        printf("Enter a Path to remove folder\n");
        return 0;
    }
    char *str = cwd;
    strcat(str, args[1]);
    if(rmdir(str)==-1)
        perror("Mad_shell: ");

    return 1;
}

int mad_ls(char **args)
{
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(cwd)) == NULL)
        printf("can’t open %s", cwd);

    while ((dirp = readdir(dp)) != NULL) {
        struct stat fileStat;
        stat(dirp->d_name,&fileStat);
        printf("%s\n", dirp->d_name);
        if(args[1]!=NULL) {

            printf("————————————————————————\n");
            printf("File Size: \t\t%llu bytes\n",fileStat.st_size);
            printf("Number of Links: \t%d\n",fileStat.st_nlink);
            printf("File inode: \t\t%llu\n",fileStat.st_ino);

            printf("File Permissions: \t");
            printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
            printf("\n");

            printf("The file %s a symbolic link\n\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
        }
    }
    printf("\n");
    return 1;
}

int mad_cp(char **args) {

    int BUFF_SIZE=1024;
    int srcFD,destFD,nbread,nbwrite;
    char *buff[BUFF_SIZE];

    if(strcmp(args[1], "--help")==0)
    {
        printf("\nUsage: cp source_file destination_file\n");
        return 0;
    }

    srcFD = open(args[1],O_RDONLY);

    if(srcFD == -1)
    {
        printf("\nError opening file %s errno = %d\n",args[1],errno);
        return 0;
    }

    destFD = open(args[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    if(destFD == -1)
    {
        printf("\nError opening file %s errno = %d\n",args[2],errno);
        return 0;
    }

    /*Start data transfer from src file to dest file till it reaches EOF*/
    while((nbread = read(srcFD,buff,BUFF_SIZE)) > 0)
    {
        if(write(destFD,buff,nbread) != nbread)
            printf("\nError in writing data to %s\n",args[2]);
    }

    if(nbread == -1)
        printf("\nError in reading data from %s\n",args[1]);

    if(close(srcFD) == -1)
        printf("\nError in closing file %s\n",args[1]);

    if(close(destFD) == -1)
        printf("\nError in closing file %s\n",args[2]);

    return 1;
}

int mad_help(char **args)
{
    int i;
    printf("********************************************\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("  Madhav's Shell\n");
    printf("  Type the commands with arguments required and press enter\n");
    printf("  The following are built in\n");

    for(i=0; i<mad_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("\nUse the man command for information on other programs\n\n");
    printf("********************************************\n");
    printf("********************************************\n");
    printf("********************************************\n");
    return 1;
}

int mad_env(char **args)
{
    if(args[1]==NULL) {
        printf("Required Env Variable\n");
        return 0;
    }
    if(args[2]==NULL) {
        printf("What to add to this variable?\n");
        return 0;
    }
    char *str = getenv(args[1]);
    strcat(str,":");
    strcat(str,args[2]);
    setenv("PATH",str, 1);
    system("echo $PATH");

    return 1;
}

int mad_exit(char **args)
{
    return 0;
}

static int mad_parse_launch(char *cmd_exec, int input, int first, int last)
{
    char *new_cmd_exec1;
    new_cmd_exec1=strdup(cmd_exec);

    int m=1;
    int i;
    args[0]=strtok(cmd_exec," ");
    while((args[m]=strtok(NULL," "))!=NULL)
        m++;
    args[m]=NULL;
    if (args[0] != NULL)
    {

        // if (strcmp(args[0], "exit") == 0)
        //         exit(0);
        if (strcmp(args[0], "echo") != 0)
        {
            cmd_exec = skipcomma(new_cmd_exec1);
            int m=1;
            args[0]=strtok(cmd_exec," ");
            while((args[m]=strtok(NULL," "))!=NULL)
                m++;
            args[m]=NULL;

        }

        //checking for builtin commands
        for (i = 0; i < mad_num_builtins(); i++) {
            // printf("%s %s\n",args[0],builtin_str[i]);
            if (strcmp(args[0], builtin_str[i]) == 0) {
                return (*builtin_func[i])(args);
            }
        }
        int arg_len = strlen(args[0]);
        if(args[0][arg_len-1]== '&')
        {
            args[0][arg_len-1] = '\0';
            // printf("Pushing to background\n");
            background=1;
        }
    }
    // if(background==1) return mad_execute_background(args);
    return mad_execute(input, first, last, new_cmd_exec1);
}


void mad_parse_line()
{

    int i, n=1, input, first;

    input=0;
    first= 1;

    cmd_exec[0]=strtok(input_buffer,"|");

    while ((cmd_exec[n]=strtok(NULL,"|"))!=NULL)
        n++;
    cmd_exec[n]=NULL;
    pipe_count=n-1;
    for(i=0; i<n-1; i++) {
        input = mad_parse_launch(cmd_exec[i], input, first, 0);
        first=0;
    }
    input=mad_parse_launch(cmd_exec[i], input, first, 1);
    input=0;

    return;

}

void prompt()
{
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("MadShell::");
        printf("%s> ",cwd);
    }
    else
        perror("getcwd() error");

}

int main()
{
    char ch[2]= {"\n"};
    getcwd(current_directory, sizeof(current_directory));
    signal(SIGINT, sigintHandler);
    init_shell();

    while (1)
    {
        clear_variables();
        prompt();
        fgets(input_buffer, 1024, stdin);
        if(strcmp(input_buffer, ch)==0)
        {
            continue;
        }
        len = strlen(input_buffer);
        input_buffer[len-1]='\0';
        strcpy(his_var, input_buffer);
        if(strcmp(input_buffer, "exit") == 0)
        {
            flag = 1;
            break;
        }
        mad_parse_line();

    }
    if(flag==1)
    {
        printf("Closing...\n");
        exit(0);
        return 0;
    }
    return 0;
}
