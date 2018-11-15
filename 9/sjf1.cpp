#include <iostream>
#include <limits.h>
#include <vector>
#include <unistd.h>

using namespace std;

#define pb push_back
#define clear() printf("\033[H\033[J")

class process
{
public:
	vector< int> times;
	int p;
};

int main()
{

	int n,t=0;
	int temp,i;

	cin>>n;

	process* plist = new process[n];
	int *turnaround_time = new int[n];
	int *waiting_time = new int[n];

	for(i=0;i<n;i++)
	{	
		//printf("For %d process\n",i);
		plist[i].p=0;
		temp=0;

		while(temp!=-1) {cin>>temp;plist[i].times.pb(temp);}
		plist[i].times.pb(temp);

	}

	int active=-1;
	int min_cpu;

	for(i=0;i<n;i++)
	{
		int p=1;
		waiting_time[i]=0;
		turnaround_time[i]=0;
		while(plist[i].times[p]!=-1 && p%2==1)
			waiting_time[i]+=plist[i].times[p++];
	}

	while(1)
	{
		//check for newly arrived processes and processes in i/o time
		for(i=0;i<n;i++)
		{
			if(plist[i].p==0 && plist[i].times[0]==t) 
				plist[i].p++;

			if(plist[i].p!=0 && plist[i].times[plist[i].p]!=-1 && plist[i].p%2==0 && !--plist[i].times[plist[i].p])
				plist[i].p++;
		}
 
		//now if cpu is free, find the shortest cpu burst job to run
		if(active==-1)
		{
			//check if all processes are completed
			for(i=0;i<n;i++){
				if(plist[i].times[plist[i].p]!=-1) break;
				else if(turnaround_time[i]==0) turnaround_time[i]=t-plist[i].times[0];
			}

			if(i==n) break; //completed all processes

			min_cpu=INT_MAX; //initially shortest job is the first job
			for(i=0;i<n;i++)
			{
				if(plist[i].p!=0 && plist[i].times[plist[i].p]!=-1 && plist[i].p%2==1)
				{
					if(plist[i].times[plist[i].p]<min_cpu){
						active=i;
						min_cpu=plist[i].times[plist[i].p];
					}
				}
			}
		
		}

		//check for completion of active cpu burst
		else if(!--plist[active].times[plist[active].p]){

				plist[active].p++;
				active=-1;

				//print the current status
				clear();
				cout<<"\nAt time "<<t<<endl;
				for(i=0;i<n;i++)
				{	
					int p=0;
					while(plist[i].times[p]!=-1)
					{
						if(plist[i].p == p) cout<<"("<<plist[i].times[p]<<") ";
						else cout<<plist[i].times[p]<<" ";
						p++;
					}
					cout<<endl;
				}
				sleep(1);
				
		}

		t++;
	}

	printf("\n::: Report :::\n");
	for(i=0;i<n;i++){
		waiting_time[i]=turnaround_time[i]-waiting_time[i];
		printf("Process %d Turnaround time :%d and Waiting time:%d\n",i,turnaround_time[i],waiting_time[i]);
	}
	int sum1=0,sum2=0;
	for(i=0;i<n;i++)
	{	
		sum1+=turnaround_time[i];
		sum2+=waiting_time[i];
	}
	printf("Average Turnaround time: %d\nAverage Waiting time: %d\nThroughput: %f\n",sum1/n,sum2/n,float(n/float(t)));

	return 0;

}