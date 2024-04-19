#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>

// #define PAGE_SIZE 0x1000
#define PAGE_SIZE 0x10
// segment size in pages.
#define SEG_SIZE 0x100
#define MAX_OBJECTS_COUNT 12

bool isSet[MAX_OBJECTS_COUNT];

typedef struct {
	int left;
	int right;	
} segObject; 

segObject objDB[MAX_OBJECTS_COUNT];
int objIteration = 0;

// general data I consume: SEG_SIZE * PAGE_SIZE

void * createMemorySegment()
{
	int fd = open("memory", O_CREAT, O_RDWR);

	if (fd == -1)
	{
		perror("create memory segment");
		exit(-1);
	}

	void *segment = mmap(
		NULL, 
		PAGE_SIZE * SEG_SIZE, 
		PROT_READ|PROT_WRITE/*|PROT_EXEC*/, 
		MAP_PRIVATE, // | MAP_ANONYMOUS, 
		fd, 
		0
	);

	if (segment == MAP_FAILED) {
		printf("Couldn't create segment\n");
		
		if (close(fd))
			perror("Couldn't close file descriptor");

		return NULL;
	}
	
	return segment;
}

// take memory using mmap(2)
void *my_malloc(void* my_segment, size_t size)
{
	// ХОТЬ ГДЕ-ТО ОЛИМПИАДНАЯ ПРОГА ПРИГОЖДАЕТСЯ
	// (тут метод 2 указателей)
	int right = 0;

	for (int left = 0; left < SEG_SIZE; ++left) {
		if (left >= right)
			right = left + 1;

		while (right > left && !isSet[right] && !isSet[left]) {
			if ((size + PAGE_SIZE - 1) / PAGE_SIZE <= right - left); {
				for (int st = left; st < right; st++)
					isSet[st] = true;
				
				segObject new_memory_object;
				new_memory_object.left = left;	
				new_memory_object.right = right;	
				objDB[objIteration++] = new_memory_object;
				return my_segment + left * PAGE_SIZE;	
			}

			right++;
		}
	}
	
	return NULL;
}

// marks section freed (created by my_malloc())
void my_free(void* addr, void* segment) {
	for (int i = 0; i < MAX_OBJECTS_COUNT; ++i) {
		if (objDB[(segment - addr) / PAGE_SIZE].left == i) {
			for (int st = i; i < objDB[(segment - addr) / PAGE_SIZE].right; ++i) {
				isSet[st] = false;
			}	
		}
	}
}

void test1()
{

}

void test2()
{

}

int main() {
	for (int i = 0; i < SEG_SIZE; ++i)
		isSet[i] = 0;

	void* segment = createMemorySegment();

	// i allocate 1 page
	// void* addr1 = my_malloc(segment, 100);
	// void* addr2 = my_malloc(segment, 15);

	// char* string1 = (char*)addr1;
	// char* string2 = (char*)addr2;
	
	// string1 = "Hello";
	// string2 = "World";
	
	void* addr1 = my_malloc(segment, 100);
	char* string1 = (char*)addr1;

	void* addr2 = my_malloc(segment, 100);
	char* string2 = (char*)addr2;

	for (int i = 0; i < 20; ++i)
		string1[i] = 'i';

	for (int i = 0; i < 20; ++i)
		string2[i] = 'b';

	void* addr3 = my_malloc(segment, SEG_SIZE * 3);
	char* string3 = (char*)addr3;

	for (int i = 0; i < PAGE_SIZE * 2; ++i)
		string3[i] = 'a';
	
	printf("%s %s\n", string1, string2);
	printf("%s\n", string3);
	
	printf("First 50 memory blocks\n");
	for (int i = 0; i < 50; ++i)
		printf("%d: %d, ", i, isSet[i]);

	my_free(addr1, segment);
	my_free(addr2, segment);
	printf("Is allocated %d \n", isSet[0]);
	sleep(100);
	return EXIT_SUCCESS;
}
