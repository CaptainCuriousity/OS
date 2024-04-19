#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_NAME 256
#define PAGE_SIZE 0x1000
#define ull unsigned long long

int hasInfo(uint64_t data)
{
	if (((data >> 55) & 1) || ((data >> 61) & 1) || ((data >> 62) & 1) || ((data >> 63) & 1))
		return 1;
	else
		return 0;

}

void printPage(uint64_t address, uint64_t data) {
	printf("Address in virtual memory: 0x%-16lx: pfn|swap_type %-16lx soft-dirty %ld file/shared %ld "
			"swapped %ld present %ld\n",
			address,
			data & 0x7fffffffffffff,
			(data >> 55) & 1,
			(data >> 61) & 1,
			(data >> 62) & 1,
			(data >> 63) & 1);
}

// unsuccessful attempt to get offset starting from the very first section of process
uint64_t getOffset(const char* pid)
{
	char *name = malloc(MAX_NAME * sizeof(char));
	strcat(name, "/proc/");
	strcat(name, pid);
	strcat(name, "/maps");
	
	char *first_12 = malloc(13 * sizeof(char));
	int fd = open(name, O_RDONLY);
	if (pread(fd, first_12, 12, 0) != 12)
		perror("getOffset");


	uint64_t result = 0;
	for (int i = 0; i < 12; ++i){
		if ('0' <= first_12[i] && first_12[i] <= '9')
			result = result * 16 + (first_12[i] - '0');
		else
			result = result * 16 + (first_12[i] - 'a' + 10);
	}
	
	return result;
}

void printPagemap(const char* pid)
{
	char *name = malloc(MAX_NAME * sizeof(char));
	strcat(name, "/proc/");
	strcat(name, pid);
	strcat(name, "/pagemap");


	int fd = open(name, O_RDONLY); // O_RDONLY means read only
	if(fd < 0) {
		perror("Opening of pid file gone wrong");
		return;
	}
	
	// off_t offset = 0x7db08055d000;
	off_t offset = getOffset(pid);
	// printf("0x%-16lx ::: %-16lx\n", offset, offset2);
	for(ull i = offset; i < 0xffffffffffff; i += PAGE_SIZE)
	{ 
		ull data;
		ull index = (i / PAGE_SIZE) * sizeof(data);
		if (pread(fd, &data, sizeof(data), index) != sizeof(data))
		{
			perror("pread"); 
			break;
		}
		// printPage(i, data);	
		if (hasInfo(data))
		{
			printPage(i, data);
			usleep(200000);
		}

		if (i % 0x100000000 == 0)
			printf("0x%-16lx\n", i);
	}
	close(fd);
	free(name);
}

int main(int argc, char *argv[]) 
{
	if(argc != 2) 
	{
		printf("wrong number of arguments");
		return 1;
	}
	printPagemap(argv[1]);
	
	return 0;
}
