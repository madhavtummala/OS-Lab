#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <time.h>

using namespace std;

int pageFaultsLRU(int pages[], int n, int capacity) 
{ 
    unordered_set<int> s; 
  
    unordered_map<int, int> indexes; 
 
    int page_faults = 0; 
    for (int i=0; i<n; i++) 
    {    
        if (s.size() < capacity) 
        {   
            if (s.find(pages[i])==s.end()) 
            { 
                s.insert(pages[i]);            
                page_faults++; 
            } 
        
            indexes[pages[i]] = i; 
        } 
  
        else
        {
            if (s.find(pages[i]) == s.end()) 
            { 
                int lru = INT_MAX, val; 
                for (auto it=s.begin(); it!=s.end(); it++) 
                { 
                    if (indexes[*it] < lru) 
                    { 
                        lru = indexes[*it]; 
                        val = *it; 
                    } 
                } 

                s.erase(val);
                s.insert(pages[i]); 

                page_faults++; 
            } 
  
        
            indexes[pages[i]] = i; 
        } 
    } 
  
    return page_faults; 
}

int pageFaultsFCFS(int pages[], int n, int capacity) 
{ 
    unordered_set<int> s;
    queue<int> indexes; 

    int page_faults = 0; 
    for (int i=0; i<n; i++) 
    { 
        if (s.size() < capacity) 
        { 
            if (s.find(pages[i])==s.end()) 
            { 
                s.insert(pages[i]); 
                page_faults++; 
            
                indexes.push(pages[i]); 
            } 
        } 

        else
        {
            if (s.find(pages[i]) == s.end()) 
            { 
                int val = indexes.front(); 
  
                indexes.pop(); 
  
                s.erase(val); 
                s.insert(pages[i]); 

                indexes.push(pages[i]); 

                page_faults++; 
            } 
        } 
    } 
  
    return page_faults; 
}


bool search(int key, vector<int>& fr) 
{ 
    for (int i = 0; i < fr.size(); i++) 
        if (fr[i] == key) 
            return true; 
    return false; 
}

int predict(int pg[], vector<int>& fr, int pn, int index) 
{ 

    int res = -1, farthest = index; 
    for (int i = 0; i < fr.size(); i++) { 
        int j; 
        for (j = index; j < pn; j++) { 
            if (fr[i] == pg[j]) { 
                if (j > farthest) { 
                    farthest = j; 
                    res = i; 
                } 
                break; 
            } 
        } 
  

        if (j == pn) 
            return i; 
    } 
  
    return (res == -1) ? 0 : res; 
} 
  
int optimalPage(int pg[], int pn, int fn) 
{ 

    vector<int> fr; 

    int hit = 0; 
    for (int i = 0; i < pn; i++) { 
   
        if (search(pg[i], fr)) { 
            hit++; 
            continue; 
        } 
   
        if (fr.size() < fn) 
            fr.push_back(pg[i]); 
  
        
        else { 
            int j = predict(pg, fr, pn, i + 1); 
            fr[j] = pg[i]; 
        } 
    } 
    
    return pn - hit;
} 

int main()
{
    // int n,p;
    // int faults=0;
    // cout<<"Enter the no of requests\n";
    // cin>>n;
    // int arr[n];

    // cout<<"Enter the no of slots\n";
    // cin>>p;

    // cout<<"Enter the requests\n";
    // for(int i=0;i<n;i++)
    //     cin>>arr[i];

    srand(time(NULL));
    int n=20;

    int arr[n];

    for(int i=0;i<n;i++)
        arr[i]=rand()%20;

    for(int p=1;p<=20;p++)
    {
        cout<<pageFaultsFCFS(arr,n,p)<<" ";
        cout<<pageFaultsLRU(arr,n,p)<<" ";
        cout<<optimalPage(arr,n,p)<<" ";
        cout<<endl;
    }

    cout<<endl;

    return 0;
}