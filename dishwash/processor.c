#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include "../split/split.h"
#include "config.h"

void remove_newline(char* string){
	if (string[strlen(string)-1]=='\n') string[strlen(string)-1]=0;
}
unsigned int hash(unsigned char *str)
{
    unsigned int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
int lineCount(FILE* fp){
	char ch;
	int lines;
	while(!feof(fp))
	{
		ch = fgetc(fp);
		if(ch == '\n')
		{
			lines++;
		}
	}
	rewind(fp);
	return lines;
}
typedef struct hashlist_ hashlist;
struct hashlist_{
	unsigned int hash;
	unsigned int value;
	hashlist* next;
};
void insertInHashlist(hashlist** hashList, unsigned int hash, unsigned int value){
	if(!*hashList){
		*hashList=calloc(1, sizeof(hashlist));
		(*hashList)->hash=hash;
		(*hashList)->value=value;
		return;
	}
	if(hash<=(*hashList)->hash){
		hashlist* tmp;
		tmp=(*hashList);
		*hashList=calloc(1, sizeof(hashlist));
		(*hashList)->hash=hash;
		(*hashList)->value=value;
		(*hashList)->next=tmp;
		return;
	}
	insertInHashlist(&((*hashList)->next), hash, value);
}
hashlist* readProcessingTimes(char* path){
	hashlist* res=NULL;	
	FILE* fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int i, argsCount, ssize;
	fp=fopen(path, "r");
	if (fp == NULL){
		printf("Error: Cannot open file.");
		exit(-1);
	}
	
	args = malloc(MAX_ARGS * sizeof(char*));
	for(i = 0; i < MAX_ARGS; i++) args[i] = malloc(MAX_CHARS_ARG * sizeof(char));

	while ((read = getline(&line, &len, fp)) != -1) {
		remove_newline(line);
		argsCount = split(line, args, ":");
		if(argsCount>2){
			printf("Error: Wrong input file format.");
			exit(-1);
		}
		insertInHashlist(&res, hash(args[0]), atoi(args[1]));
	}
	for(i = 0; i < MAX_ARGS; i++) free(args[i]);
	free(args);
	fclose(fp);
	if (line) free(line);
	return res;
}
int getProcessingTime(hashlist* processingTimes, unsigned int hash){
	hashlist* processingTimes_;
	processingTimes_=processingTimes;
	while(1){
		if(!processingTimes_) break;
		if(processingTimes_->hash==hash) return processingTimes_->value;
		if(processingTimes_->hash>hash) break;
		processingTimes_=processingTimes_->next;
	}
	
	printf("Error: Item could not be identified.\n");
	exit(-1);
	return -1;
	

}

void run(hashlist* processingTimes, int fd_in, int fd_out, char* action){
	char * line = NULL;
	int i, num, ssize, time;
	unsigned int exit_hash, cur_hash;
	const int one = 1;
	line=calloc(MAX_CHARS_ARG, sizeof(char*));
	exit_hash=hash("exit");
	while (1) {
		read(fd_in, &ssize, sizeof(ssize));
		read(fd_in, line, (ssize)*sizeof(char));
		read(fd_in, &num, sizeof(int));
		line[ssize]=0;
		cur_hash=hash(line);
		if(cur_hash==exit_hash) time=0;
		else time=getProcessingTime(processingTimes, cur_hash);
		for(i=0; i<num; i++){
			printf("%s %s...\n", action, line);
			sleep(time);
			write(fd_out, &ssize, sizeof(int));
			write(fd_out,line,ssize*sizeof(char));
			write(fd_out, &one, sizeof(int));
			printf("Sent down the conveyor.\n");
		}
		if(cur_hash==exit_hash){
			printf("Stopping %s machine.\n", action); 
			break;
		}
	}
	if (line) free(line);
	return;
	
}


void freeHashList(hashlist* hashList){
	if(!hashList) return;
	freeHashList(hashList->next);
	free(hashList);
}

int main(int argc, char **argv){
	hashlist* processingTimes;
	int fd_in, fd_out;
	if (argc!=5){
		printf("Error: Wrong number of arguments\n");
		exit(-1);
	}

	processingTimes=readProcessingTimes(argv[1]);
	if (access(argv[2], F_OK)==-1 && 
		mknod(argv[2], S_IFIFO | 0666, 0) < 0)
	{
		printf("Error: No permission to create or access FIFO.\n");
		exit(-1);
	}
	if (access(argv[3], F_OK)==-1 && 
		mknod(argv[3], S_IFIFO | 0666, 0) < 0)
	{
		printf("Error: No permission to create or access FIFO.\n");
		exit(-1);
	}
	fd_in= open(argv[2], 'r');
	fd_out= open(argv[3], O_WRONLY);
	run(processingTimes, fd_in, fd_out, argv[4]);
	freeHashList(processingTimes);
	close(fd_in);
	close(fd_out);

	return 0;
}
