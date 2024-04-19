#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// int main(int argc, char* argv[], char* envp[])

#define MAX_VAR_SIZE 32
#define MAX_COM_SIZE 256

void menu()
{
	printf("Custom env var editor\n");
	printf("Options: p(rint)|s(et)|c(reate)|h(elp)|e(xit) \n");
	char option = '\0';
	char* buffer1 = (char*)malloc(MAX_VAR_SIZE * sizeof(char));
	char* buffer2 = (char*)malloc(MAX_VAR_SIZE * sizeof(char));

	while(1)
	{
		printf("Var edit >> ");
		option = getc(stdin);
		switch (option)
		{
			case 'p':
				printf("Type name of var to print >> ");
				scanf("%s", buffer1);
				printf("Value of %s: %s\n", buffer1, getenv(buffer1));
				break;
			case 's':
				printf("Type name of var to set >> ");
				scanf("%s", buffer1);
				printf("Type value to set %s with >> ", buffer1);
				scanf("%s", buffer2);

				char* temp = (char*)malloc(MAX_COM_SIZE * sizeof(char));
				strcat(temp, buffer1);
				strcat(temp, "=");
				strcat(temp, buffer2);
				printf("Resulting command: %s\n", temp);

				if (putenv(temp))
					perror("Error during change of var");

				printf("%s value: %s\n", buffer1, getenv(buffer1));

				free(temp);
				break;
			case 'c':
				printf("Type name of var to create >> ");
				scanf("%s", buffer1);

				if (putenv(buffer1))
					perror("Error during creation of var");

				break;
			case 'h':
				printf("Options: p(rint)|s(et)|h(elp)|e(xit) \n");
				break;
			case 'e':
				printf("Finish\n");
				return;
				break;
			default:
				break;

		}
	}
	free(buffer1);
	free(buffer2);
}

int main(int argc, char* argv[])
{
	menu();
	// putenv("TEST=TEST");
	// printf("%s\n", getenv("TEST"));	
	return 0;
}
