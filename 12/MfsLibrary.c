#include "MfsLibrary.h"
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define fAttrSize 11 //in bytes 8+3
#define fNameSize 8
#define fOffsSize 3

//scans the disk for existing file or opens new file.
filehandler myfopen(char *filename)
{
	int disk=open("Mfs.dat", O_RDWR, S_IRUSR | S_IWUSR);

	if(disk<0) {printf("Disk not present\n");return -1;}

	char *fAttr = (char*)malloc(sizeof(char)*fAttrSize);
	char *fName = (char*)malloc(sizeof(char)*fNameSize);
	int Offs;
	char fEnd[8] = "00000000";
	char fFree[8] = "freearea";

	while(1)
	{
		if(read(disk, (void*)fAttr, fAttrSize)==-1)
		{
			printf("Error in read\n");
			return -1;
		}
		memcpy((void*)fName, (void*)fAttr, fNameSize);
		memcpy((void*)(&Offs), (void*)(fAttr+fNameSize), fOffsSize);

		if(strcmp(filename,fName)==0)
			return disk;

		if(strcmp(fEnd,fName)==0)
			break;

		if(lseek(disk,Offs,SEEK_SET)<0)
		{
			printf("No File and not space.\n");
			return -1;
		}
	}

	printf("File not found. Creating new file\n");
	if(write(disk,(void*)filename,fNameSize)!=fNameSize)
	{
		printf("Error in writing filename\n");
		return -1;
	}

	printf("%d\n",disk);
	return disk;

}