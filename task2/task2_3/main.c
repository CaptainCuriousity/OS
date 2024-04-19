#include <stdio.h>
#include <sys/ptrace.h>
#include <stdlib.h>

// by default, syscall wrappers return -1 if error happened
#define ERRCODE -1

void trace()
{
	ptrace(PTRACE_TRACEME);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Wrong number of arguments\n");
		exit(1);
	} else
		trace();
	return 0;
}
