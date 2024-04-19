#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const int ARR_SIZE = 1024 * 10;

void killStack()
{
	int a[ARR_SIZE];
	printf("Current pid: %d\n", getpid());
	sleep(1);
	killStack();
}

int main()
{
	printf("Current pid: %d\n", getpid());
	int a[ARR_SIZE];
	sleep(10);
	killStack();
	return 0;
}
