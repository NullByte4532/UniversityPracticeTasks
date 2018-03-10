#include <dirent.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILENAME_LEN 128
#define MAX_FINDINGS 16
void search(char* dir, char* file, int max_findings, char** res, int recursive_flag){
	DIR *d;
	int len;
	static int num_found;
	struct dirent *item;
	struct stat path_stat;
	d=opendir(dir);
	char path[MAX_FILENAME_LEN];
	strcpy(path, dir);
	len=strlen(path);
	path[len] = '/';
	path[len + 1] = 0;
	if(!recursive_flag) num_found = 0;
	if(max_findings <= num_found) return;
	if (d){
		while ((item = readdir(d)) != NULL){
			strcat(path, item->d_name);
			stat(path, &path_stat);
			if(S_ISREG(path_stat.st_mode)){
				if(!strcmp(item->d_name, file)){
					
					res[num_found] = calloc(strlen(path)+1, sizeof(char));
					strcpy(res[num_found], path);
					num_found++;
					if(max_findings <= num_found) return;
				}

			}
			if(S_ISDIR(path_stat.st_mode) && item->d_name[0] != '.'){
				search(path, file, max_findings, res, 1);
				if(max_findings <= num_found) return;
			}
			path[len + 1] = 0;
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
	char** res;
	int i;
	res=calloc(MAX_FINDINGS, sizeof(char*));
	search(argv[1], argv[2], MAX_FINDINGS, res, 0);
	for(i=0; i<MAX_FINDINGS; i++)
		if(res[i])
			printf("%d\t%s\n", i, res[i]);
	for(i=0; i<MAX_FINDINGS; i++)
		if(res[i])
			free(res[i]);
	free(res);
	return 0;
}
