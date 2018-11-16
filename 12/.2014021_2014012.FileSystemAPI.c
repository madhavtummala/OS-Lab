#include <.2014021_2014012.FileSystemAPI.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
	File System Offsets	in Bytes
*/
#define superOffset  0 * 1024
#define inodeBitmapOffset   4 * 1024
#define dataBitmapOffset  8 * 1024
#define inodeDataOffset  12 * 1024
#define dataOffset  ( 12 + 4 * 128 ) * 1024

int createSFS(char* filename, int nbytes){
	/*Return values :
	-1 : File not created
	-2 : Error while writing to file 
	+ve value : File created as expected
	*/
	int return_value=open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); //Add O_CREAT
	if(return_value<0) return -1;
	int i,err;
	char data=0;
	for(i=0;i<nbytes;i++)
	{
		err=write(return_value,(void*)(&data),1);
		if(err!=1) return -2;
	}
	char* super_data;
	char sentimental[]="eyeNodeSFS";
	super_data=(char*)malloc(sizeof(char)*(strlen(sentimental)+1));
	strcpy(super_data,sentimental);
	writeData(return_value,0,(void*)super_data);
	return return_value;	
}

int readData(int disk, int blockNum, void* block)
{
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	 +ve number : Data read successfully
	*/
	int fourKB=4*1024;
	if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	if(read(disk,block,fourKB)!=fourKB) return -2; //4KB Data Block
	return fourKB;
}

int writeData(int disk, int blockNum, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in write()
	 +ve number : Data written successfully
	*/
	 int fourKB=4*1024;
	 if(lseek(disk,blockNum*fourKB,SEEK_SET)<0) return -1;
	 if(write(disk,block,fourKB)!=fourKB) return -2; //4KB Data Block
	 return fourKB;
}

int writeFile(int disk, char* filename, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	-3 : No space to write data
	-4 : No space to write inode entry
	-5 : Error writing to file
	 0 : File created successfully
	*/
	
	int actual_size,block_size,sixteenB; 
	int j,i,k,blockNum,fourKB,data_space,inode_space;
	fourKB=4*1024;
	sixteenB=16;

	//Needs to be checked :
	actual_size=0;
	while(*(char*)(actual_size+block)!=0) actual_size++; 
	block_size=actual_size/fourKB;
	if(actual_size%fourKB != 0) block_size++;

	// //Temporary :
	// block_size=1;
	// actual_size=fourKB;

	data_space=-1;
	char inode_space_block,data_space_block;
	inode_space=-1;
	char* dataBitmap;
	blockNum=dataBitmapOffset;
	dataBitmap=(char*)malloc(sizeof(char)*fourKB); //4KB data bitmap
	if(lseek(disk,blockNum,SEEK_SET)<0) return -1;
	if(read(disk,(void*)dataBitmap,fourKB) == -1){
		// printf("Data Bitmap%s\n", (char*)dataBitmap);
		return -2;
	}
	// printf("First checkpoint\n");
	//Find space in data_bitmap
	int maxContFound = 0;
	for(i = 0; i < fourKB; i++)
	{
		for(j = 0;j < 8; j++)
		{
			k = (unsigned char)dataBitmap[i];
			// printf("Data bitmap is %d\n",k);
			int isSet = (128 >> j) & k;
			// printf("IsSet is %d\n",isSet);
			if (isSet == 0) maxContFound++;
			else maxContFound = 0;
			if(maxContFound==block_size)
			{
				data_space = (8 * i + j) - ( maxContFound -1);
				data_space_block = dataBitmap[ data_space / 8];
				goto heaven;
			}
		}
	}
	heaven:
	if(data_space == -1) return -3; //No space for data
	//Find space for inode entry in inode_bitmap
	char* inodeBitmap;
	blockNum = inodeBitmapOffset;
	inodeBitmap = (char*)malloc(sizeof(char)*fourKB); //4KB inode bitmap
	if(lseek(disk,blockNum,SEEK_SET) < 0) return -1;
	if(read(disk,(void*)inodeBitmap,fourKB) == -1) return -2;
	// printf("Second checkpoint\n");
	for(i=0;i<fourKB;i++)
	{
		for(j=0;j<8;j++)
		{
			k=(unsigned char)inodeBitmap[i];
			// printf("I read %d\n",k);
			k=(128>>j)&k;
			if(k==0)
			{
				inode_space=(8 * i + j); 
				inode_space_block=inodeBitmap[inode_space / 8]; //Get 8 bytes
				goto hell; 
			}
		}
	}	
	unsigned char yoda;
	int jedi;
	hell:
	if(inode_space==-1) return -4; //No space for inode entry
	//TESTED:
		//Set inode bitmap (inode_space)  to one:
		yoda=inode_space_block;
		jedi=(int)yoda;
		// printf("Before : %d, want to %dth bit\n",jedi,(inode_space%8));
		jedi=((128)>>(inode_space%8)) | jedi; //Setting 'inode_space%8'th bit
		// printf("After : %d\n",jedi);
		// printf("Inode_space/8 is %d\n",inode_space/8);
		if(lseek(disk,inodeBitmapOffset+(inode_space/8),SEEK_SET)<0) return -1; 
		yoda=(char)jedi;
		if(write(disk,(void*)(&yoda),1)!=1) return -2; //Rewriting that whole byte (as it is tedious to rewrite individual bit)
		// printf("Third checkpoint\n");

	//Checking if inode bit was actually set
	// unsigned char into;
	// lseek(disk,inodeBitmapOffset+(inode_space/8),SEEK_SET);
	// read(disk,(void*)(&into),1);
	// printf("Verified : %d\n",into);

	//TESTED:
		//Set data bitmap (data_space) to one {for now..need to set all data maps for input data}:
		yoda=data_space_block;
		jedi=(int)yoda;
		// printf("Before : %d, want to %dth bit\n",jedi,(inode_space%8));
		jedi=((128)>>(data_space%8)) | jedi; //Setting 'data_space%8'th bit
		// printf("After : %d\n",jedi);
		if(lseek(disk,dataBitmapOffset+(data_space/8),SEEK_SET)<0) return -1;
		// printf("Data_space/8 is %d\n",data_space/8); 
		yoda=(char)jedi;
		if(write(disk,(void*)(&yoda),1)!=1) return -2; ////Rewriting that whole byte (as it is tedious to rewrite individual bit)
		// printf("Fourth checkpoint\n");

	// // Checking if inode bit was actually set
	// unsigned char into;
	// lseek(disk,inodeBitmapOffset+(inode_space/8),SEEK_SET);
	// read(disk,(void*)(&into),1);
	// printf("was there %d\n",into);

	//TESTED : 
		//Write metadata to inode table
		if(lseek(disk,inodeDataOffset+inode_space*sixteenB,SEEK_SET)<0) return -1;
		if(write(disk,(void*)filename,8)!=8) return -2; //Setting 8 byte filename
		// printf("Fifth checkpoint\n");

	// //Checking if filename was actually written:
	// lseek(disk,inodeDataOffset+inode_space*fourKB,SEEK_SET); 
	// char* holla;
	// holla=(char*)malloc(sizeof(char)*8);
	// read(disk,(void*)holla,8);
	// int yoy=0;
	// printf("%s WTAF\n",holla);

	char* ex;
	ex=(char*)malloc(sizeof(char)*2);
	memcpy((void*)ex,(void*)(&data_space),2); //Copying starting block of file (data)
	if(write(disk,(void*)ex,2)!=2) return -2; //Setting 2 byte starting block of file
	// printf("Sixth checkpoint\n");
	//free(ex);
	memcpy((void*)ex,(void*)(&block_size),2); //Copying number of blocks for file (data)
	if(write(disk,(void*)ex,2)!=2) return -2; //Setting 2 byte size of file (in blocks)
	// printf("Seventh checkpoint\n");
	//free(ex);
	ex=(char*)malloc(sizeof(char)*4);
	memcpy((void*)ex,(void*)(&actual_size),4); //Copying actual file size (data)
	if(write(disk,(void*)ex,4)!=4) return -2; //Setting 4 blocks for file size
	// printf("Eigth checkpoint\n");

	//Write data
	char* one_block;
	one_block=(char*)malloc(sizeof(char)*fourKB);
	// if(lseek(disk,dataOffset+(data_space*fourKB),SEEK_SET)<0) return -1; //Seek pointer to location where data is to be written
	// printf("Ninth checkpoint\n");

	//Write 4KB at at time
	for(i=0;i<block_size;i++)
	{
		memcpy(one_block,((char*)block),fourKB);
		if(writeData(disk,(dataOffset/fourKB)+i+data_space,(void*)one_block)<0) return -5; //Error writing to file
		block=block+fourKB; //4KB at a time
	}
	return 0;
}

int readFile(int disk, char* filename, void* block){
	/*Return values :
	-1 : Error in lseek()
	-2 : Error in read()
	-3 : File not found (in inode table) error
	-4 : Error reading from file
	 0 : Data read successfully
	*/
	int i,found;
	int fourKB=1024*4;
	int starting,file_size,n_blocks;
	char *name,*word;
	word=(char*)malloc(sizeof(char)*16);
	name=(char*)malloc(sizeof(char)*8); 
	found=0;
	for(i=inodeDataOffset;i<dataOffset;i+=16)
	{
		if(lseek(disk,i,SEEK_SET)<0) return -1;
		if(read(disk,(void*)word,16)!=16) return -2; //4KB Data Block
		memcpy((void*)name,(void*)word,8); //Extract file name
		// printf("Read : %s\n",name);
		if(strcmp(name,filename) == 0)
		{
			found=1;
			memcpy((void*)(&starting),(void*)(word+8),2);
			memcpy((void*)(&n_blocks),(void*)(word+10),2);
			memcpy((void*)(&file_size),(void*)(word+12),4);
			printf("Starting block : %d\n",starting);
			printf("Number of blocks : %d\n",n_blocks);
			printf("Size of file : %dB\n",file_size);
			break;
		}
	}	
	if(!found) return -3;
	int starting_block=starting+(dataOffset/fourKB); //Block 'number'
	// printf("Starting block %d\n",starting_block);
	if(lseek(disk,starting_block,SEEK_SET)<0) return -1; //Seek to starting of file
	char* one_block;
	one_block=(char*)malloc(sizeof(char)*fourKB);
	//Reading 4KB at a time :
	for(i=0;i<n_blocks;i++)
	{
		if(readData(disk,starting_block+i,(void*)one_block) == -1) return -4;
		memcpy((void*)(block+i*fourKB),(void*)one_block,fourKB); //Adding data to block, 4KB at a time
	}
	// read(disk,block,file_size);
	return 0;
}