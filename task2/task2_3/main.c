#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

void child(int argc, char **argv) {
	char *buf [argc + 1];
	memcpy(buf, argv, argc * sizeof(char*));
	buf[argc] = NULL;

	// PT_TRACE_ME means that this process is traced by its parent
	// PT_TRACE_ME is used only by the tracee
	ptrace(PT_TRACE_ME);
	kill(getpid(), SIGSTOP);

	// execution of argument:
	execvp(buf[0], buf);
}

int waitSyscall(pid_t pid){
	int status;
	while (1) {
		// 
		ptrace(PTRACE_SYSCALL, pid, 0, 0);

		waitpid(pid, &status, 0);
		
		// it means: child process was stopped correctly, we continue.
		if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
			return 0;
		// WIFEXITED returns a nonzero value if process terminated with exit signal.
		if (WIFEXITED(status))
			return 1;
	}
}

void parent(pid_t pid) {
	int waitStatus, syscall;
	waitpid(pid, &waitStatus, 0);

	// PTRACE_SETOPTIONS means setting ptrace options
	// from data argument (4th argument)
	// PTRACE_O_TRCESYSGOOD means something to work with
	// system call traps?
	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	
	while(!waitSyscall(pid)) {
		// here I take syscall number (sic!)
		syscall = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX);
		fprintf(stderr, "syscall(%d) = ", syscall);
		if(waitSyscall(pid) != 0) 
			break;
		// here i take result after syscall (sic!)x2
		int result = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * RAX);
		fprintf(stderr, "%d\n", result);

		sleep(10);
	}
}

int main(int argc, char **argv) {
	
	if (argc < 2) {
		printf ("wrong number of arguments\n");
		return EXIT_FAILURE;
	}
	pid_t pid = fork();
	if (pid == 0)
		// meaning: i throw out the frist argument (argc[0]=="./a.out")
		child(argc-1, argv+1);
	else if (pid == -1) {
		printf("can't create process\n");
		return EXIT_FAILURE;
        }
        else
		parent(pid);
        return EXIT_SUCCESS;
}
