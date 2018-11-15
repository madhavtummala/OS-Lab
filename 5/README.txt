Name: T.Madhav
Roll: 16CS01041

Assignment 5 (Hare and Turtle Race)

Using Pipes(Lab Assignment):

Used named pipes, for more flexibility.

Steps:

chmod +x run.sh (to add execute permission)
./run.sh

In another terminal (to send signal to invoke god and reposition hare and turtle) 
chmod +x sig.sh (to add execute permission)
./sig.sh

Using Threads (Home Assignment):

Used pthreads (POSIX Threads).

Steps:

gcc race.c -o race
./race

Press Ctrl + Z to invoke god process and reposition.

Input in God Process:

h <new hare position>
t <new turtle position> 
q (to quit)

Defaults:

hare speed : 5
hare sleeptime = 2
hare separation tolerance = hare speed * 2
turtle speed = 1
control sleep time = 1s (else everyting will complete in ms)

Refer Screen shots in each folder for more help.