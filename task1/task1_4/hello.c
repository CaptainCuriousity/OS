#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include "dynhello.h"

int main()
{
	// void* handle = dlopen("./dynhello.so", RTLD_NOW);
	hello_from_dynamic_lib();
	// dlclose(handle);
	return 0;
}
