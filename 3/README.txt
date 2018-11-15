NAME : T.Madhav
ROLL : 16CS01041

Assignment 3

The screen shots show the test cases used.

There are two versions of the assignment. 
In src-manualpid, the entire process not only runs with the same pid, but we can also choose what will be that pid. So we can basically run the programs under the pid we want. This must be run with root permissions.

Steps:
In src folders:

gcc p1.c -o twice 
gcc p2.c -o square 
gcc p3.c -o half
gcc ass3.c

./a.out <command>  for src

OR

./a.out <PID> 
<command>          for src-manualpid

