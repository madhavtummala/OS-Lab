#include <iostream>
#include <list>

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
			queue.pop_front();
			queue.push_back(arr[i]);
			cout<<endl;
			for(it=queue.begin();it!=queue.end();++it)
				cout<<*it<<" ";
			cout<<endl;
		}

	}

	faults+=p;
	cout<<"No of Page Faults "<<faults<<endl;

	return 0;
}