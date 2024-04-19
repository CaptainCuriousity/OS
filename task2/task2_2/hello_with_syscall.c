#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

void swrite(int fd, void* buffer, size_t size)
{
	syscall(SYS_write, fd, buffer, size);
	printf("%d\n", SYS_write);
}

int main()
{
	swrite(1, "Hello World!\n", 14);
	return 0;
}
