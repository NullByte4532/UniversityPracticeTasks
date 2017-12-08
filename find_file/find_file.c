#include <dirent.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILENAME_LEN 128
void search(char* dir, char* file){
	DIR *d;
	int len;
	struct dirent *item;
	struct stat path_stat;
	d=opendir(dir);
	char path[MAX_FILENAME_LEN];
	strcpy(path, dir);
	len=strlen(path);
	path[len]='/';
	path[len+1]=0;
	if (d){
		while ((item = readdir(d)) != NULL){
			strcat(path, item->d_name);
			stat(path, &path_stat);
			if(S_ISREG(path_stat.st_mode)){
				if(!strcmp(item->d_name, file)) printf("%s\n", path);

			}
			if(S_ISDIR(path_stat.st_mode)&& item->d_name[0]!='.'){
				search(path, file);
			}
			path[len+1]=0;
		}
		closedir(d);
	}else{
		printf("Error listing directory %s\n", dir);
		exit(-1);
	}

}
int main(int argc, char** argv)
{
	if(argc!=3){
		printf("Error: wrong number of arguments\n");
		exit(-1);
	}
	search(argv[1], argv[2]);
	return 0;
}
