#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a_glob;
int b_glob = 10;
const int c_glob = 10;

void* create_var()
{
	int local = 13121;
	printf("Local var in func create_var() %p\n", local);
	return (void*)&local;
}

void create_buffer()
{
	char* buffer = (char*)malloc(120);
	for (int i = 0; i < 10; ++i)
		buffer[i] = '0' + i;
	printf("%s\n", buffer);
	free(buffer);
	printf("%s\n", buffer);
	
	char* buffer2 = (char*)malloc(120);

	for (int i = 0; i < 10; ++i)
		buffer[i] = '9' - i;
	printf("%s\n", buffer);
	buffer += 60;
	// error!
	free(buffer);
	printf("%s\n", buffer);
}

int main(int argc, char** argv)
{
	printf("Cur pid%d\n", getpid());
	int a;
	static int b;
	const int c = 10;

	// printf("Local, usual: %p\n", (void*)&a);
	// printf("Local, static: %p\n", (void*)&b);
	// printf("Local, const: %p\n", (void*)&c);
	
	// printf("global uninit: %p\n", (void*)&a_glob);
	// printf("global init: %p\n", (void*)&b_glob);
	// printf("global const: %p\n", (void*)&c_glob);

	printf("Local variable out of function create_var(): %p\n", create_var());
	create_buffer();	
	sleep(1000);
	return 0;
}
