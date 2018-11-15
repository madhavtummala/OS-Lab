#include <iostream>
#include <string>

using namespace std;

int main()
{
	int t,n;
	string str;
	char temp;

	cin>>t;

	while(t--)
	{
		cin>>n;
		cin>>str;

		for(int i=0;i<str.length()-1;i+=2)
		{
			temp=str[i];
			str[i]=str[i+1];
			str[i+1]=temp;
		}

		for(int i=0;i<str.length();i++)
			str[i] = 'a'+'z'-str[i];

		cout<<str<<endl;
	}

	return 0;

}