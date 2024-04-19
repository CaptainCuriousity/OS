#include <stdio.h>
#include <unistd.h>

// take memory using mmap(2)
void *my_malloc(size_t size)
{
	// PRIVATE <=> 
	uint64_t *p == mmap(NULL, size, PROT_READ|PROT_WRITE/*|PROT_EXEC*/, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (p == MAP_FAILED)
		return NULL;
	return p;
}

// marks section freed (created by my_malloc())
void *my_free()
{

}

int main()
{
	return 0;
}
