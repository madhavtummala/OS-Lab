# Mfs - Simple File System

Roll no - 16CS01041<br />
File System Assignment<br />

### Details
MfsLibrary.h - Custom file system library
MfsLibrary.c - Custom Library Implementation
MfsFormat.c - A Program to format the 100KB file system
Mfs.dat - The complete file system with its contents
Test[1-3].c - Test files
run.sh - A helper script to run the complete test

### Instruction
```
chmod +x run.sh
./run.sh
cat Mfs.dat //for seeing the current status of FS
```

### Logic
This is a basic file system. The allocation is continous.
File Attributes - 11 B (8B Filename + 3B Offset to next file)
File Data - upto 100 KB

#### fopen
For opennig a file, we match each existing filename with the given name and if it matches, return the file pointer or create a new file at the end of FS.

#### fread
For reading, we simply state reading given bytes or max bytes of file data and store it in buffer.

#### fwrite
If the file is freshly created, we update the file size and store the file data. If existing file is overflowed by larger data, we find a continous free space that can allocated the new size or create at the end of FS, marking the current space as free.

#### fclose
close(fd)

### Remarks
There is possibility of *internal fragmentation* here. This problem can be removed by implementing block structure and maintaining inodes and thier data to continous blocks.

Even in this kind of inode structure the allocation of free space will be continous, so there will be *external fragmentaion.* To avoid this, we can use inode structue which points to each individual block of the file, hence the file is no longer continuos. This achieves maximum usage of disk.

To find the free spaces availiable a *inode and data bitmaps* of 8 bit numbers are maintained. Each bit of each number corresponds to the status of one block. So the size of bitmap is determined by the no of blocks and hence the block size fixed for the FS. Typical block size is 4 KB.
