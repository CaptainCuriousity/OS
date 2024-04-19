#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>

#define PAGE_SIZE 0x1000
#define SIZE 10

const int from_index = 4;
const int to_index = 6;

void* getPages(void){
	void *addr = mmap(NULL, SIZE * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		perror("Error during getting region");
		exit(EXIT_FAILURE);
	}
	printf("Data address: %p\n", addr);
	// sleep(60);
	return addr;
}

void closeRegion(void *addr){
	if (munmap(addr, SIZE * PAGE_SIZE) == -1){
		perror("Error during closure of pages");
		exit(EXIT_FAILURE);
	}
}

void changeMode(void){
	void *addr = getPages();
	printf("anonymous region begin = %p\nregion size = %d pages (%d bytes)\n", addr, SIZE, SIZE * PAGE_SIZE);

	
	// this piece works correctly	
	*((char*)addr) = 'c';
	printf("%c\n", *((char*)addr));

	if (mprotect(addr, SIZE * PAGE_SIZE, PROT_WRITE) == -1) {
		perror("Error during check for writing");
	}
	
	printf("reading: %s\n", (char*)addr);
	if (mprotect(addr, SIZE * PAGE_SIZE, PROT_READ) == -1) {
		perror("Error during check for reading");
	}

	// writing (causes segfault)
	*((char*)addr) = 'c';
}

void handler(int sig){
	printf("SIGSEGV detected\n");
	signal(sig, SIG_IGN);
	exit(-1);
}

void delete46(void){
	void *reg = getPages();
	printf("region = %p\n", reg);
	//sleep(20);
	reg += from_index * PAGE_SIZE;
	sleep(10);
	munmap(reg, (to_index - from_index) * PAGE_SIZE);
	printf("deleted part = %p\n", reg);
	//sleep(20);
	
	// gives random trash
	printf("Trying to read from deleted area:  %d\n", reg);
	sleep(5);
}

int main()
{
	printf("Pid of this process: %d\n", getpid());
	signal(SIGSEGV, &handler);

	// TEST 1: creation and deletion
	void* pages = getPages();	
	if (pages != NULL)
		printf("Pages were created\n");

	closeRegion(pages);
	printf("If you see this message, then it closed successfully\n");

	// TEST 2: change mode (causes segfault)
	// changeMode();
		
	// TEST 3: cause segfault manually
	// int *intptr;
	// *intptr = 12;	
	
	// TEST 46: delete from 4th to 6th
	printf("waitint 30\n");
	sleep(10);
	delete46();
	sleep(10);

	return EXIT_SUCCESS;
}
