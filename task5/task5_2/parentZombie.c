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
	pid_t pid2;
	int tmp;
	switch(pid=fork()) {
		case -1:
			perror("can't create new process\n");
			exit(-1);
		case 0:
			printf("\n\nFirst generation child (will be zombified)");

			switch(pid2=fork())
			{
				case -1:
					perror("Can't create new process\n");
					exit(-1);
				case 0:
					printf("\n\nSecond generation child\n\n");
					printf("Pid of 2nd generation: %d\n", getpid());
					sleep(50);
					break;
				default:
					sleep(1);
					printf("First generation child happily commits suicide!");
					exit(10);
					break;		
			}
			exit(5);
		default:
			printf("\n\nParent process info: START\n\n");
			printf("\tGlobal: address = %p, content = %d\n", &global, global);
			printf("\tLocal: address = %p, content = %d\n", &local, local);
			
			// sleeping to work with zombie child	
			sleep(100);
			// parent process waits until the end of execution of child process
			wait(&tmp);
			printf("\texit code of child = %d\n", WEXITSTATUS(tmp));

			printf("Variables after execution of child\n");
			printf("\tGlobal: address = %p, content = %d\n", &global, global);
			printf("\tLocal: address = %p, content = %d\n", &local, local);
			printf("\n\nParent process info: END\n\n");
			break;
	}
}

int main() {
	int local = 1234;
	printf("Main function info: \n");
	printVars(global, local);
	pid_t parent = getpid();
	printf("pid = %d\n", parent);
	printf("End of main function info, createProc->\n");
	createProc(local);
	sleep(30);
	return EXIT_SUCCESS;
}
