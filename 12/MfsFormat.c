#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FSsize 100*1024

int main()
{
	int disk=open("Mfs.dat", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if(disk<0) {printf("Disk Problem\n");return -1;}

	char data = 0;
	int i;

	for(i=0;i<FSsize;i++)
	{
		if(write(disk,(void*)(&data),1)!=1)
		{
			printf("File Format Error\n");
			return -1;
		}
	}

	close(disk);

	return 0;
}