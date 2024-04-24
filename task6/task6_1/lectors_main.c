#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define SIZE 128

int main(int argc, char *argv[])
{
	int sh_fd;
	void *sh_mem;
	char *buff;
	char val = 0;

	sh_fd = open("./shared_memory", O_RDWR | O_CREAT, 0660);
	ftruncate(sh_fd, 0);
	ftruncate(sh_fd, SIZE);  // what is this?
	
	sh_mem = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sh_fd, 0);
	close(sh_fd);

	buff = (char*)sh_mem;
	while(1)
	{
		for (int i = 0; i < SIZE; ++i)
		{
			buff[i] = val++;
			sleep(1);
		}

	}

	return EXIT_SUCCESS;
}
