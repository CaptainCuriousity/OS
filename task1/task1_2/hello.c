#include <stdio.h>
#include <unistd.h>
#include "libhello.h"

int main()
{
	hello_from_static_lib();
	sleep(10);
	return 0;
}
