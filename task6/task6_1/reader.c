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

#define DEFAULT_SIZE 75

// данная программа была ПОЛНОСТЬЮ переписана с лекции
int main(int argc, char *argv[]) {
	// printf("%d", getpagesize());
	int size;	
	if (argc == 1)
		size = DEFAULT_SIZE;		
	else if (argc == 2)
		size = atoi(argv[1]);
			

	fflush(stdout);

	int sh_fd;
	void *sh_mem;
	char *buff;
	char val = 0;

	sh_fd = open("./shared_memory", O_RDWR | O_CREAT, 0660);
	ftruncate(sh_fd, 0);
	ftruncate(sh_fd, size);
	
	sh_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, sh_fd, 0);

	// закрытие файла ни к чему плохому не приводит. Оно лишь означает, что
	// нельзя будет выполнять read/write.
	close(sh_fd);

	buff = (char*)sh_mem;

	char prev_val = -1;
	while(1) {
		for (int i = 0; i < size / sizeof(char); ++i) {
			val = buff[i];
			if (val - prev_val != 1) {
				printf("Something wrong: %d, %d", prev_val, val);
			}
			
			// printf("%d ", val);
			prev_val = val;
			fflush(stdout);
			usleep(1);
		}

	}
	
	munmap(sh_mem, size);

	return EXIT_SUCCESS;
}
