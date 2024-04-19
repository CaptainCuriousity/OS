#define _XOPEN_SOURCE 700
#include <fcntl.h> 
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

typedef unsigned long long ull;

typedef struct {
    uint64_t pfn : 54;
    unsigned int soft_dirty : 1;
    unsigned int file_page : 1;
    unsigned int swapped : 1;
    unsigned int present : 1;
} PagemapEntry;

/* Parse the pagemap entry for the given virtual address.
 *
 * @param[out] entry      the parsed entry
 * @param[in]  pagemap_fd file descriptor to an open /proc/pid/pagemap file
 * @param[in]  vaddr      virtual address to get entry for
 * @return 0 for success, 1 for failure
 */
int pagemap_get_entry(PagemapEntry *entry, int pagemap_fd, uintptr_t vaddr)
{
	printf("in func\n");
	size_t nread;
	ssize_t ret;
	uint64_t data;

	nread = 0;
	while (nread < sizeof(data)) 
	{
		printf("ok");
		ret = pread(pagemap_fd, ((uint8_t*)&data) + nread, sizeof(data) - nread,
				(vaddr / sysconf(_SC_PAGE_SIZE)) * sizeof(data) + nread);
		// ret = pread(pagemap_fd, (uint8_t*)&data, sizeof(data), 0);

		printf("ok");
		if (ret <= 0)
			printf("shit happened\n");

		nread += ret;
		if (ret <= 0) {
			return 1;
		}
	}
	entry->pfn = data & (((uint64_t)1 << 54) - 1);
	entry->soft_dirty = (data >> 54) & 1;
	entry->file_page = (data >> 61) & 1;
	entry->swapped = (data >> 62) & 1;
	entry->present = (data >> 63) & 1;
	return 0;
}

void printEntry(PagemapEntry* entry)
{
	if (entry == NULL)
		return;
	printf("S/D: %llu, Filepage: %u, Swapped: %u, Present: %u, Address: %u", entry->soft_dirty, entry->file_page, entry->swapped, entry->present, entry->pfn);
}

int main(int argc, char** argv)
{
	PagemapEntry* entry;
	
	printf("Everything is ok\n...");	
	int fd = open("/proc/1/pagemap", O_RDONLY);

	printf("%d", fd);
	pagemap_get_entry(entry, fd, 0);
	printEntry(entry);
	
	return 0;
}
