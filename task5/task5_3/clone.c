#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SEG_SIZE 0x1000

void cloning() {
	printf("%d", getpid());

	int fd;

	fd = open("memory", O_RDWR | O_CREAT, 0660);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	ftruncate(fd, 0x0);
	ftruncate(fd, SEG_SIZE);
	void* segment = mmap(NULL, SEG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);



	int* array = (int*)segment;

	for (int i = 0; i < 3; ++i)
		array[i] = i;
	
	// int* array = (int*)segment;
	// for (int i = 0; i < 10; ++i)
		// array[i] = i;

	// for (int i = 0; i < 10; ++i)
	// {
		// printf("%d ",*array);	
		// array++;
	// }
	// printf("\n");
		
	// sleep(1000);	
}

void entryPoint() {

}

void H4LL0ST4CK() {

}

int main() {
	cloning();
	return EXIT_SUCCESS;
}
