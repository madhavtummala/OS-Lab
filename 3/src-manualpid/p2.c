#include <stdio.h>
#include <stdlib.h>

int main()
{
	int a;
	FILE *fp = fopen("temp.txt","r");
	fscanf(fp,"%d",&a);
	fp = freopen("temp.txt","w",stdout);
	fprintf(fp,"%d",a*a);
	return 0;
}