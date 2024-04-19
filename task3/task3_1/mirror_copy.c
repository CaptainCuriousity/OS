#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/errno.h>

#define MAX_PATH 512

const char path1 [MAX_PATH] = "/home/cursed_nerd/nsu/os/task3/task3_1/";
const char path2 [MAX_PATH] = "/home/cursed_nerd/nsu/os/task3/task3_1/";
const char dirname [MAX_PATH] = "testFolder";

char* reverse(const char* str)
{
	size_t length = strlen(str);
	char* new_string = calloc(length, sizeof(char));
	for (size_t i = 0; i < length; ++i)
		new_string[i] = str[length - i - 1];

	return new_string;
}

void rewrite(const char *name, const char *workPath, const char *newPath){
	printf("Rewriting file %s from %s\ninto %s\n", name, workPath, newPath);
	char c;
	char *buf = reverse(name);
	char *tmp = calloc(MAX_PATH, sizeof(char));
	strcpy(tmp, workPath);
	
	FILE *oldFile;
	if (oldFile = fopen(strcat(tmp, name), "rb"))
	{
		printf("Error during opening of file %s ", tmp);
		if (errno == EACCES) printf("(access denied)\n");
		if (errno == ENOENT) printf("(incorrect path)\n");

		exit(1);
	}


	strcpy(tmp, newPath);
	FILE *newFile = fopen(strcat(tmp, buf), "w");
	int i = -1;

	while (fseek(oldFile, i, SEEK_END) != -1)
	{
		c = fgetc(oldFile);
		// printf("%c", c);
		fputc(c,newFile);
		--i;
	}
	if (fclose(oldFile))
	{
		printf("Error during closing of file %s", tmp);
		if (errno == EBADF)
			printf("(bad file descriptor)\n");
		exit(1);
	}
	fclose(newFile);
	free(buf);
	free(tmp);
	printf("Rewriting file %s succeeded\n\n", name, workPath, newPath);
}

void createDir(const char* name, char* path)
{
	char* buf = reverse(name);
	strcat(path, buf);
	mkdir(path, 0777);
	strcat(path, "/");
	free(buf);
}

void mirrorCopy(const char *name, const char *workPath, const char *newPath)
{
	printf("Mirrorcopying %s from %s\ninto %s\n", name, workPath, newPath);

	char *tmp = calloc(MAX_PATH, sizeof(char));
	char *tmp2 = calloc(MAX_PATH, sizeof(char));
	strcpy(tmp, workPath);
	DIR *current = opendir(strcat(tmp, name));
	struct dirent *entry;

	char *curPath = calloc(MAX_PATH, sizeof(char));
	strcpy(curPath, tmp);
	strcat(curPath,"/");
	strcpy(tmp2, newPath);
	createDir(name, tmp2);

	while ((entry = readdir(current)) != NULL) 
	{
		if(entry->d_type == DT_REG)
		{
			rewrite(entry->d_name, curPath, tmp2);
		}
		else if (entry->d_type == DT_DIR)
		{
			if (strcmp(entry->d_name, "..") && strcmp(entry->d_name, "."))
			{
				mirrorCopy(entry->d_name, curPath, tmp2);
			}
		}
	};
	printf("Mirrorcopying %s finished\n\n", name);
	closedir(current);
	free(entry);
	free(tmp);
	free(tmp2);
}

int main()
{
	mirrorCopy(dirname, path1, path2);
	return 0;
}
