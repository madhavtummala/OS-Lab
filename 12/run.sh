gcc Test1.c MfsLibrary.c -I. -o test1
gcc Test2.c MfsLibrary.c -I. -o test2
gcc Test3.c MfsLibrary.c -I. -o test3
gcc MfsFormat.c -o format
./format
./test1 & ./test2 & ./test3