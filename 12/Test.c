#include <.2014021_2014012.FileSystemAPI.h>
#include <.2014021_2014012.DiagnosticAPI.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
	int option,size,ret;
	char *data,*beta;
	char* filename=(char*)malloc(sizeof(char*)*1000); 
	printf("Enter name of SFS to be created\n");
	scanf("%s",filename);
	strcat(filename,".dat");
	printf("Enter file size of desired SFS (in MB)\n");
	scanf("%d",&size);
	size*=1024;
	int file_descriptor=createSFS(filename,size);
	if(file_descriptor<0)
	{
		printf("Error : Error creating SFS.Exiting..\n");
		return;
	}
	else
	{
		printf("SFS created.\n");
	}
	//Change from 4KB to something else
	data=(char*)malloc(sizeof(char*)*4*1024); //4KB block
	beta=(char*)malloc(sizeof(char*)*4*1024); //4KB block
	printf("Welcome to eyeNode UI\n");
	do
	{
		printf("1.Create new file\n");
		printf("2.Open file\n");
		printf("3.Display inode bitmap\n");
		printf("4.Display data bitmap\n");
		printf("5.Display information of all files on disk\n");
		printf("6.Exit\n");
		scanf("%d",&option);
		switch(option)
		{
			case 6: printf("Exiting.\n");
					break;
			case 5 : print_FileList(file_descriptor);
					break;
			case 4 : print_dataBitmaps(file_descriptor);
					break;
			case 3 : print_inodeBitmaps(file_descriptor);
					break;
			case 1 : printf("Enter desired filename\n");
					 scanf("%s",beta);
					 printf("Enter file content\n");
					 scanf("%s",data);
					 ret=writeFile(file_descriptor,beta,(void*)data);
					 printf("Returned %d\n",ret);
					 //Error handling for ret
					 break;
			case 2 : printf("Enter filename\n");
					 scanf("%s",beta);
					 ret=readFile(file_descriptor,beta,(void*)data);
					 //Error handling for ret
					 printf("Data read : \n%s\n",data);
					 printf("Returned %d\n",ret);
					 break;
			default : printf("Invalid option\n");

		}
	}while(option!=6);
}