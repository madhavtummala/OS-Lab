#include <stdio.h>
#include <unistd.h>

int main()
{
	while(1)
	{
		sleep(1);
		printf("infinite loop :)\n");
	}

	return 0;
}