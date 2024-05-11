#define _GNU_SOURCE
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sched.h>
#include <linux/sched.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <string.h>

#define SEG_SIZE 0x1000
#define DEFAULT_DEPTH 10

long long sp;
int fd;

void* createStack() {
	fd = open("memory", O_RDWR | O_CREAT, 0660);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	ftruncate(fd, 0x0);
	ftruncate(fd, SEG_SIZE);
	void* stack = mmap(
			NULL,
		       	SEG_SIZE, 
			PROT_READ | PROT_WRITE, 
			MAP_SHARED, 
			fd, 
			0
	);

	if (stack == MAP_FAILED) {
		perror("Stack failed");
		exit(EXIT_FAILURE);
	}

	sp = (long long)stack;
	return stack;
}

void doStackJob(int depth) {
	const char hello_world[] = "Hello World!";
	if (depth <= 0)
		return;

	printf("Current stack position: %p\n", sp);

	char* charred_stack = (char*)sp;

	for (int i = 0; i < 13; ++i)
		charred_stack[i] = hello_world[i];

	sp += strlen(hello_world);
	
	void* return_address = __builtin_extract_return_addr(__builtin_return_address(0));
	printf("Return address of doStckJob(%d): %p\n", depth, return_address);
	printf("Depth: %p\n", &depth);
	doStackJob(depth - 1);
	
}

int entryPointChild(void* stack) {
	doStackJob(DEFAULT_DEPTH);
	exit(EXIT_SUCCESS);
	return 0;
}

int main() {
	void* stack = createStack();
	void* stack_top = stack + SEG_SIZE;

	printf("Stack: %p, Stack top: %p\n", stack, stack_top);

	pid_t pid = clone(entryPointChild, stack_top, SIGCHLD, NULL);
	if (pid == -1) {
		perror("Clone failed");
		return EXIT_FAILURE;
	}
	printf("Pid of cloned process: %d\n", pid);
	printf("Current pid: %d\n", getpid());
	return EXIT_SUCCESS;
}
