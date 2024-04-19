#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <string.h>


void printfile(const char* filename)
{
	FILE* f; // = fopen(filename, "r");
	if (!(f = fopen(filename, "r")))
	{
		perror("Error during opening of file");
		exit(1);
	}

	char fileInfo[1024];
	fgets(fileInfo, 1024, f);
	printf("%s", fileInfo);
	
	if (fclose(f))
	{
		printf("Error during closing of file %s", filename);
		if (errno == EBADF)
			printf("(bad file descriptor");
		exit(1);
	}
}

int main()
{
	printfile("testfile.txt");
	uid_t userid = getuid();
	uid_t efid = geteuid();
	
	printf("%d\n%d\n", userid, efid);
	return 0;
}
