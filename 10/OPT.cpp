#include <iostream>
#include <list>
#include <limits.h>

using namespace std;

int main()
{
	int n,p;
	int faults=0;
	cout<<"Enter the no of requests\n";
	cin>>n;
	int arr[n];

	cout<<"Enter the no of slots\n";
	cin>>p;

	cout<<"Enter the requests\n";
	for(int i=0;i<n;i++)
		cin>>arr[i];

	list<int> queue;

	for(int i=0;i<p;i++)
		queue.push_back(arr[i]);

	faults+=p;

	list<int>::iterator it;
	cout<<endl;

	cout<<endl;
	for(it=queue.begin();it!=queue.end();++it)
		cout<<*it<<" ";
	cout<<endl;

	for(int i=p;i<n;i++)
	{	
		bool found=false;
		for(it=queue.begin();it!=queue.end();++it){
			if(*it==arr[i])
				found=true;
		}

		if(found) continue;
		else{

			faults++;

			int far=INT_MIN;
			list<int>::iterator replace=queue.begin();
			for(it=queue.begin();it!=queue.end();++it)
			{
				//cout<<"Start\n";
				int j;
				for(j=i+1;j<n;j++){
					//cout<<arr[j]<<" "<<*it<<endl;
					if(arr[j]==*it){
						//cout<<"Found "<<arr[i]<<"at "<<j<<endl;
						if(j>far){
							far=j;
							replace=it;
						}
						break;
					}
				}
				if(j==n) {replace=it;break;}
				//cout<<"Ended\n";
			}
			
			*replace=arr[i];

			cout<<endl;
			for(it=queue.begin();it!=queue.end();++it)
				cout<<*it<<" ";
			cout<<endl;
		}

	}

	cout<<"No of Page Faults "<<faults<<endl;

	return 0;
}