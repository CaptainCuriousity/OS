#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("Cur pid %d\n", getpid());
	for (int i = 0; i < 60; ++i)
	{
		printf("%d\n", i);
		sleep(1);
	}
	return 0;
}
