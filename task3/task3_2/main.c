#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

// char* pwd = "/proc/64";

const size_t MAX_PATH = 512;
const size_t MAX_BUF = 512;

void createDirectory(const char* pwd, const char* name) {
	char *namedpath = (char*)calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);

	struct stat st = {0}; // what is it?
	if (!mkdir(namedpath, 0700))
		printf("Folder %s was successfully created\n", namedpath);
	else
	{
		perror("Error during creation of folder");
		exit(1);
	}

	free(namedpath);
}

void listDirectory(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	
	DIR *current = opendir(namedpath);
	if (current == NULL) {
		perror("Error during opening of folder");
		printf("%s\n", namedpath);
		exit(1);
	}

	struct dirent *entry;
	while (entry = readdir(current))
		printf("%s %d\n", entry->d_name, entry->d_type);

	closedir(current);
	free(entry);
	free(namedpath);
}

void deleteDirectory(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	
	if (!rmdir(namedpath))
		printf("Directory %s was successfully deleted", namedpath);
	else
	{
		printf("Error during deletion of %s directory", namedpath);
	}

	free(namedpath);
}

void createFile(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	
	if (access(namedpath, F_OK) != -1)
	{
		printf("Error: file exists\n");
		return;
	}

	FILE* file = fopen(namedpath, "w");

	if (file)
		printf("File %s was created\n", namedpath);
	else
	{
		perror("Error during creation of file");
	}
		
	free(namedpath);


	if (fclose(file))
	{
		perror("Error during closing of file");
	}
}

void showFile(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);

	FILE *file = fopen(namedpath, "rb");

	if (!file)
	{
		// printf("Error during opening of file %s\n", namedpath);
		perror("Error during opening of file (show file func)");
		return;
	}

	char c;
	while ((c = fgetc(file)) != EOF)
		printf("%c", c);


	if (fclose(file))
	{
		perror("Error during closing of file");
		exit(1);
	}
	free(namedpath);
}

void deleteFile(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);

	if (!remove(namedpath))
	{
		perror("Error during deletion of file");
		exit(1);
	}
	else
		printf("File %s was successfully deleted");

	free(namedpath);
}


void createSymbolic(const char* pwd, const char* name) {
	char* namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	
	char* symlink_name = calloc(MAX_PATH, sizeof(char));
	strcpy(symlink_name, namedpath);
	strcat(symlink_name, "_sym_link");

	if (symlink(namedpath, symlink_name))
	{
		perror("Error while creating symlink");
		exit(1);
	}
	
	free(symlink_name);
	free(namedpath);
}

void checkSymbolic(const char* pwd, const char *name){
	char *namedpath = calloc(MAX_PATH, sizeof(char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	char *buf = calloc(MAX_BUF, sizeof(char));
	readlink(namedpath, buf, MAX_BUF);
	printf("%s", buf);
	free(buf);
	free(namedpath);
}

void checkFileSymbolic(const char* pwd, const char *name){
    char *tmp = calloc(MAX_PATH, sizeof (char));
    strcpy(tmp, pwd);
    strcat(tmp, name);
    char *buf = calloc(MAX_BUF, sizeof (char));
    readlink(tmp, buf, MAX_BUF);
    FILE *fd = fopen(buf, "rb");
    if (fd == NULL){
	    perror("Error during opening of file (via symlink)\n");
	    exit(1);
    }
    char c;
    while (c = fgetc(fd)) 
        printf("%c",c);
    
    fclose(fd);
    free(buf);
    free(tmp);
}

void deleteSymbolic(const char* pwd, const char *name){
    char *buf = calloc(MAX_PATH, sizeof (char));
    strcpy(buf, pwd);
    strcat(buf, name);
    if(remove(buf))
	    printf("Error during delition of symbolic link\n");
    else
	    printf("Deletion was successful\n");
    free(buf);
}

void createHard(const char* pwd, const char *name){
	char *hard_link_name = calloc(MAX_PATH, sizeof (char));
	char *namedpath = calloc(MAX_PATH, sizeof (char));
	strcpy(namedpath, pwd);
	strcat(namedpath, name);
	
	strcpy(hard_link_name, namedpath);	
	strcat(hard_link_name, "_hard_link");

	if(link(namedpath, hard_link_name))
		printf("Error during creation of hard link %s\n", hard_link_name);
	else
		printf("Hard link %s was successfully created\n", hard_link_name);

	free(namedpath);
	free(hard_link_name);
}

void chMod(const char* pwd, const char *name){
    char *namedpath = calloc(MAX_PATH, sizeof (char));
    strcpy(namedpath, pwd);
    strcat(namedpath, name);
    printf("select mode: ");
    int mode = 0;
    scanf("%o", &mode); // o means octal
    chmod(namedpath, mode);
    free(namedpath);
}

void showMode(int mode,char *str){
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0]='d';
    if(S_ISCHR(mode)) str[0]='c';
    if(S_ISBLK(mode)) str[0]='b';
    if(mode & S_ISVTX)str[0]='t';
    if(mode & S_IRUSR)str[1]='r';
    if(mode & S_IWUSR)str[2]='w';
    if(mode & S_IXUSR)str[3]='x';
    if(mode & S_ISUID)str[3]='s';
    if(mode & S_IRGRP)str[4]='r';
    if(mode & S_IWGRP)str[5]='w';
    if(mode & S_IXGRP)str[6]='x';
    if(mode & S_ISGID)str[6]='s';
    if(mode & S_IROTH)str[7]='r';
    if(mode & S_IWOTH)str[8]='w';
    if(mode & S_IXOTH)str[9]='x';
}

void chModLink(const char* pwd, const char *name){
    struct stat info;
    char *namedpath = calloc(MAX_PATH, sizeof (char));
    char *str = calloc(11, sizeof (char));
    strcpy(namedpath, pwd);
    strcat(namedpath, name);
    stat(namedpath, &info);
    showMode(info.st_mode, str);
    printf("Permissions are: %s\n", str);
    free(str);
    free(namedpath);
}

void deleteHard(const char* pwd, const char *name){
    char *namedpath = calloc(MAX_PATH, sizeof (char));
    strcpy(namedpath, pwd);
    strcat(namedpath, name);
    if(unlink(namedpath))
	    printf("Error during deletion of hard link %s\n", namedpath);
    free(namedpath);
}

void menu(const char* pwd)
{
	printf("Current folder: %s\n", pwd);
}

int main() {
	// char* pwd = "/home/cursed_nerd/nsu/os/task3/task3_2/";
	char* pwd = "/proc/64/";
	showFile(pwd, "io");
	// listDirectory(pwd, "testname");
	// showFile(pwd, "testname");
	//chMod(pwd, "testname");
	
	// free(name);
	// free(name2);
	return 0;
}
