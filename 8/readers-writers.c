#include<stdio.h>
#include<pthread.h>
#include<dispatch/dispatch.h>

/*

wait(S){
while(S<=0);   // busy waiting
S--;
}

signal(S){
S++;
}

*/

/*
do{

//produce an item

wait(empty);
wait(mutex);

//place in buffer

signal(mutex);
signal(full);

}while(true)

do{

wait(full);
wait(mutex);

// remove item from buffer

signal(mutex);
signal(empty);

// consumes item

}while(true)

*/

dispatch_semaphore_t mutex,writeblock;
int data = 0,rcount = 0;

void *reader(void *arg)
{
    int f;
    f = ((int)arg);
    dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
    rcount = rcount + 1;
    if(rcount==1)
        dispatch_semaphore_wait(writeblock, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_signal(mutex);
    printf("Data read by the reader%d is %d\n",f,data);

    dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
    rcount = rcount - 1;
    if(rcount==0)
        dispatch_semaphore_signal(writeblock);
    dispatch_semaphore_signal(mutex);

    return NULL;
}

void *writer(void *arg)
{
    int f;
    f = ((int)arg);
    dispatch_semaphore_wait(writeblock, DISPATCH_TIME_FOREVER);
    data++;
    printf("Data writen by the writer%d is %d\n",f,data);
    dispatch_semaphore_signal(writeblock);

    return NULL;
}

int main()
{
    mutex = dispatch_semaphore_create(1);
    writeblock = dispatch_semaphore_create(1);
    int i,r,w;

    pthread_t rtid[5],wtid[5];

    for(i=0; i<=2; i++)
    {
        pthread_create(&wtid[i],NULL,writer,(void *)i);
        pthread_create(&rtid[i],NULL,reader,(void *)i);
    }
    for(i=0; i<=2; i++)
    {
        pthread_join(wtid[i],NULL);
        pthread_join(rtid[i],NULL);
    }

    return 0;
}