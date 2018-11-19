#include <iostream>

using namespace std;

bool isSafe(int *avail, int **maxm, int **allot, int **need, int n, int m) 
{
    int P=n;
    int R=m;
  
    // Mark all processes as infinish 
    bool finish[P];
    for(int i=0;i<n;i++)
        finish[i]=0; 
  
    // To store safe sequence 
    int safeSeq[P]; 
  
    // Make a copy of available resources 
    int work[R]; 
    for (int i = 0; i < R ; i++) 
        work[i] = avail[i]; 
  
    // While all processes are not finished 
    // or system is not in safe state. 
    int count = 0; 
    while (count < P) 
    { 
        // Find a process which is not finish and 
        // whose needs can be satisfied with current 
        // work[] resources. 
        bool found = false; 
        for (int p = 0; p < P; p++) 
        { 
            // First check if a process is finished, 
            // if no, go for next condition 
            if (finish[p] == 0) 
            { 
                // Check if for all resources of 
                // current P need is less 
                // than work 
                int j; 
                for (j = 0; j < R; j++) 
                    if (need[p][j] > work[j]) 
                        break; 
  
                // If all needs of p were satisfied. 
                if (j == R) 
                { 
                    // Add the allocated resources of 
                    // current P to the available/work 
                    // resources i.e.free the resources 
                    for (int k = 0 ; k < R ; k++) 
                        work[k] += allot[p][k]; 
  
                    // Add this process to safe sequence. 
                    safeSeq[count++] = p; 
  
                    // Mark this p as finished 
                    finish[p] = 1; 
  
                    found = true; 
                } 
            } 
        } 
  
        // If we could not find a next process in safe 
        // sequence. 
        if (found == false) 
        { 
            cout << "System is not in safe state"; 
            return false; 
        } 
    } 
  
    // If system is in safe state then 
    // safe sequence will be as below 
    cout << "System is in safe state.\nSafe"
         " sequence is: "; 
    for (int i = 0; i < P ; i++) 
        cout << safeSeq[i] << " "; 
  
    return true; 
} 

void request(int *avail, int **maxm, int **alloc, int *req, int n, int m, int mode, int p)
{
	int **need = new int*[n];
	for(int i=0;i<n;i++) need[i]=new int[m];

	//Calculating need matrix
    for (int i = 0 ; i < n ; i++) 
        for (int j = 0 ; j < m ; j++) 
            need[i][j] = maxm[i][j]-alloc[i][j];


    for(int j=0;j<m;j++)
    	if(req[j]>need[p][j]){
    		cout<<"Error, requested more than needed\n";
    		return;
    	}

    if(mode==0){
        for(int j=0;j<m;j++){
        	if(req[j]>avail[j]){
        		cout<<"Error, no availiability for process\n";
        		return;
        	}
        }
    }


    if(mode==0)
    {
        for(int j=0;j<m;j++){
        	avail[j]-=req[j];
        	alloc[p][j]+=req[j];
        	need[p][j]-=req[j];
        }
    }
    else
    {
        for(int j=0;j<m;j++){
            avail[j]+=req[j];
            alloc[p][j]-=req[j];
            need[p][j]+=req[j];
        }       
    }
    if(!isSafe(avail,maxm,alloc,need,n,m))
    {
        printf("Reverting back the request\n");
        if(mode==0)
        {
            for(int j=0;j<m;j++){
                avail[j]+=req[j];
                alloc[p][j]-=req[j];
                need[p][j]+=req[j];
            }
        }
        else
        {
            for(int j=0;j<m;j++){
                avail[j]-=req[j];
                alloc[p][j]+=req[j];
                need[p][j]-=req[j];
            }
        }
    }


}

int main()
{
	int n,m;
	cin>>n>>m;

	int *availiable = new int[m];
    int *req = new int[m];

	int **max = new int*[n];
	for(int i=0;i<n;i++) max[i]=new int[m];

	int **allocation = new int*[n];
	for(int i=0;i<n;i++) allocation[i]=new int[m];

	cout<<"Enter availiablity matrix\n";
	for(int i=0;i<m;i++)
		cin>>availiable[i];

	cout<<"Enter max matrix\n";
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			cin>>max[i][j];

	// cout<<"Enter allocation matrix\n";
	// for(int i=0;i<n;i++)
	// 	for(int j=0;j<m;j++)
	// 		cin>>allocation[i][j];

    int p;
    char mode;
    while(1)
    {
        cout<<"Enter a request\n";
        cin>>p;
        cin>>mode;

        if(mode=='a')
        {
            for(int i=0;i<m;i++)
                cin>>req[i];
            request(availiable, max, allocation, req, n, m, 0, p);
        }

        if(mode=='r')
        {
            for(int i=0;i<m;i++)
                cin>>req[i];
            request(availiable, max, allocation, req, n, m, 1, p);
        }
        cout<<endl;

    }

	return 0;

}