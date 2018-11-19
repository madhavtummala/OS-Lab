#include <iostream>
#include <limits.h>
#include <vector>
#include <unistd.h>
#include <queue>

using namespace std;

#define pb push_back
#define mp make_pair
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
	int i,slice=5;

	cin>>n;
	
	process* plist = new process[n];
	int *turnaround_time = new int[n];
	int *waiting_time = new int[n];
	queue< pair<int,int> >ready;

	int temp;
	for(i=0;i<n;i++)
	{	
		//printf("For %d process\n",i);
		plist[i].p=0;
		temp=0;

		while(temp!=-1) {cin>>temp;plist[i].times.pb(temp);}
		plist[i].times.pb(temp);

	}

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
			if(plist[i].p==0 && plist[i].times[0]>=t)
				ready.push(mp(plist[i].times[++plist[i].p],i));

			if(plist[i].p!=0 && plist[i].times[plist[i].p]!=-1 && plist[i].p%2==0 && !--plist[i].times[plist[i].p])
				ready.push(mp(plist[i].times[++plist[i].p],i));
			
		}

		if(!ready.empty())
		{ 
			pair<int,int> temp=ready.front();
			ready.pop();

			t+=(temp.first<slice)?temp.first:slice;
			temp.first-=slice;

			printf("Running %d left: %d\n",temp.second,temp.first);

			if(temp.first>0) ready.push(mp(temp.first,temp.second));
			else plist[temp.second].p++;
		}

		else{
			int i;
			int flag=0;
			for(i=0;i<n;i++){
				if(plist[i].times[plist[i].p]!=-1) flag=1;
				else if(turnaround_time[i]==0) turnaround_time[i]=t-plist[i].times[0];
			}
			if(i==n && flag==0) break;
			else t++;
		}

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