#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 123;

void printVars(int globalVar, int localVar) {
	printf("global: address = %p, content = %d\n", &globalVar, globalVar);
	printf("local: address = %p, content = %d\n", &localVar, localVar);
}

void createProc(int local) {
	pid_t pid;
	int tmp;
	switch(pid=fork()) {
		case -1:
			perror("can't create new process\n");
			exit(-1);
		case 0:
			printf("Child process: pid = %d\n", getpid());
			printf("Child process: parent pid = %d\n\n", getppid());
			printf("Child process: global: address = %p, content = %d\n local: address = %p, content = %d\n", &global, global, &local, local);

			local = 12345;
			global = 12345;

			printf("child process: changed variables\n global: address = %p, content = %d\n local: address = %p, content = %d\n", &global, global, &local, local);
			exit(5);
		default:
			printf("parent process: global: address = %p, content = %d\n local: address = %p, content = %d\n", &global, global, &local, local);

			// parent process waits until the end of execution of child process
			wait(&tmp);
			printf("parent process: exit code of child = %d\n", WEXITSTATUS(tmp));
	}
}

int main() {
	int local = 1234;
	printf("In function main:\n")
	printVars(global, local);
	pid_t parent = getpid();
	printf("pid = %d\n", parent);
	printf("\n\n");
	createProc(local);
	return 0;
}
