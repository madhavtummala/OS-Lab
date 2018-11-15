#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define clear() printf("\033[H\033[J")

int har_speed=5;
int har_sleep=2;
int tur_speed=1;
int control=1;
int active;
int har_pos=0;
int tur_pos=0;
int finish=100;
int sep=10;

void signal_handler(int signum){
	active=1;
}

void *Hare()
{
	while(har_pos<finish && tur_pos<finish)
		if(!active){
			if(tur_pos < har_pos-sep) sleep(har_sleep);
			else har_pos += har_speed;
			sleep(control);
		}

	return NULL;
}

void *Turtle()
{
	while(har_pos<finish && tur_pos<finish)
		if(!active){
			tur_pos += tur_speed;
			sleep(control);
		}

	return NULL;
}

void *God()
{
	char ch;
	while(har_pos<finish && tur_pos<finish)
		if(active){
			printf("Enter input\n");
			scanf(" %c",&ch);
			if(ch=='h')
				scanf("%d",&har_pos);
			else if(ch=='t')
				scanf("%d",&tur_pos);
			else if(ch=='q')
				active=0;
		}

	return NULL;
}

void *Report()
{
	int i;
	while(har_pos<finish && tur_pos<finish)
		if(!active){
			clear();

			for(i=0;i<100;i++){
				if(i==har_pos) printf("H");
				else if(i==99) printf("|");
				else printf(" ");
			}
			printf("\n");
			for(i=0;i<100;i++){
				if(i==tur_pos) printf("T");
				else if(i==99) printf("|");
				else printf(" ");
			}
			printf("\n");

			sleep(control);
		}

	if(tur_pos>=finish) printf("Turtle Wins\n");
	else printf("Hare Wins\n");

	return NULL;
}
int main()
{
	active=0;
	signal(SIGTSTP, signal_handler);

	pthread_t har,tur,god,rep;

	pthread_create(&har, NULL, Hare, NULL);
	pthread_create(&tur, NULL, Turtle, NULL);
	pthread_create(&god, NULL, God, NULL);
	pthread_create(&rep, NULL, Report, NULL);

	pthread_join(har, NULL);
	pthread_join(tur, NULL);
	pthread_join(god, NULL);
	pthread_join(rep, NULL);

	return 0;
}
