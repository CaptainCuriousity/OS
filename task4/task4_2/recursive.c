#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	printf("Pid of current_process: %d\n", getpid());
	sleep(1);
	execv("./a.out", argv);
	printf("FUUUU..\n");
	return 0;
}
