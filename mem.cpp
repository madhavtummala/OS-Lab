#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	string command;
	string token;
	int *mem;
	int max_size=0,allocate=0,addr=0,free=0;
	int *m;

	do
	{
		if(max_size) 
			printf("\nCurrent Logical Addr : %d\n\n",addr);

		getline(cin,command);
		stringstream s(command);
		getline(s,token,' ');

		cout<<endl;

		if(token=="initialize")
		{
			getline(s,token,' ');
			max_size=stoi(token);
			mem = new int[max_size];
			m = new int[max_size];

			for(int i=0;i<max_size;i++){
				mem[i]=-1;
				m[i]=0;
			}

			printf("Initialized the memory : size %d\n",max_size);
		}

		else if(token=="alloc")
		{
			getline(s,token,' ');
			allocate=stoi(token);

			if(max_size)
			{
				int k=0;

				for(int i=0;i<max_size;i++)
				{
					if(k==allocate) break;

					if(mem[i]==-1){
						k++;
					}
				}

				if(k!=allocate) printf("No space availiable\n");

				else
				{
					int k=0;
					m[addr]=1;
					for(int i=0;i<max_size;i++)
					{
						if(k==allocate) break;

						if(mem[i]==-1){
							mem[i]=addr;
							k++;
						}
					}
					printf("Allocated from %d to %d\n",addr,addr+allocate-1);
					addr+=allocate;
				}
			}
		}

		else if(token=="free")
		{
			getline(s,token,' ');
			free=stoi(token);

			if(m[free])
			{
				for(int i=0;i<max_size;i++)
				{
					if(mem[i]==free)
						mem[i]=-1;
				}
				m[free]=0;
			}
			else cout<<"Not found\n";
		}

		else if(token=="print")
		{
			cout<<"Logical Addr\tChunk\t\tPhysical Space\n";
			for(int j=0;j<max_size;j++)
			{
				if(m[j]){

					cout<<j<<"\t\t";

					int size=0;

					for(int i=0;i<max_size;i++)
						if(mem[i]==j)
							size++;

					cout<<size<<"\t\t";

					for(int i=0;i<max_size;i++){

						if(mem[i]==j){

							cout<<i<<"...";

							while(mem[i]==j) i++;

							cout<<i-1<<" ";

						}
					}

					cout<<endl;
				}
			}

		}
	}
	while(token!="quit");
}	