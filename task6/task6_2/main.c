#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>

int main() {
	int pid;
	int pfd[2];
	int err;

	if (pipe(pfd)) {
		perror("pipe() failed");
		return EXIT_FAILURE;
	}

	pid = fork();
	if (pid == 0) {
		uint32_t cur_num = 0;
		// char buff[] = "Hello World!";
		while (1) {
			printf("Child process: sending '%llu'\n", cur_num);
			write(pfd[1], &cur_num, sizeof(uint32_t));
			sleep(1);
			
			++cur_num;
		}

	} else if (pid > 0) {
		uint32_t cur_num;
		while (1) {
			read(pfd[0], &cur_num, sizeof(uint32_t));
			printf("Parent: recieved \a'%llu'\n", cur_num);
		}
	} else {
		perror("Pid failed");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
