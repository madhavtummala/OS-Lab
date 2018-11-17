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
filehandler myfopen(char *file_name)
{
	int disk=open("Mfs.dat", O_RDWR, S_IRUSR | S_IWUSR);

	if(disk<0) {printf("Disk not present\n");return -1;}

	char *fAttr = (char*)malloc(sizeof(char)*fAttrSize);
	char *fName = (char*)malloc(sizeof(char)*fNameSize);

	char *filename = (char*)malloc(sizeof(char)*fNameSize);
	memcpy(filename, file_name, fNameSize);

	int Offs,bytes=0;

	while(1)
	{
		if(read(disk, (void*)fAttr, fAttrSize)!=fAttrSize)
		{
			printf("Error in read\n");
			return -1;
		}

		lseek(disk,bytes,SEEK_SET);

		memcpy((void*)fName, (void*)fAttr, fNameSize);
		memcpy((void*)(&Offs), (void*)(fAttr+fNameSize), fOffsSize);

			
		printf("Filename: %s length: %lu\n",fName,strlen(fName));

		if(strlen(fName)==0)
			break;

		if(strcmp(filename,fName)==0)
			return disk;

		if(lseek(disk,Offs,SEEK_CUR)<0)
		{
			printf("No File and not space.\n");
			return -1;
		}
		bytes+=11;
		bytes+=Offs;
	}

	printf("File not found. Creating new file\n");

	if(write(disk,(void*)filename,fNameSize)!=fNameSize)
	{
		printf("Error in writing filename\n");
		return -1;
	}

	lseek(disk,bytes,SEEK_SET);
	return disk;
}

int myfread(void *out, int howmanybytes, filehandler fh)
{
	lseek(fh, fNameSize, SEEK_CUR);

	int bytes;

	char* ex = (char*)malloc(sizeof(char)*fOffsSize);

	if(read(fh, (void*)ex, fOffsSize)!=fOffsSize)
	{
		printf("Error in reading\n");
		return -1;
	}

	memcpy((void*)(&bytes), (void*)ex, fOffsSize);

	if(bytes<howmanybytes)
	{
		printf("File has only %d bytes, reading %d bytes\n",bytes,bytes);
		howmanybytes=bytes;
	}

	// if(write(disk,(void*)ex,fOffsSize)!=fOffsSize)
	// {
	// 	printf("Error in writing\n");
	// 	return -1;
	// }

	if(read(fh, out, howmanybytes)!=howmanybytes)
	{
		printf("Error in reading\n");
		return -1;
	}

	printf("Read data: %s\n", (char*)out);

	lseek(fh, 0, SEEK_SET);

	return howmanybytes;
}