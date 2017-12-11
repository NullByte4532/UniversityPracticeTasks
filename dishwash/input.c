#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include "../split/split.h"
#include <sys/sem.h>
#include "config.h"

void remove_newline(char* string){
	if (string[strlen(string)-1]=='\n') string[strlen(string)-1]=0;
}
int main(int argc, char **argv){
	char *filename_1;
	FILE * fd_in;
	int fd_out;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int i, argsCount, ssize;
	int desc;
	int semid;
	struct sembuf sb;
	if (argc!=3){
		printf("Error: Wrong number of arguments\n");
		exit(-1);
	}
	if (access(filename_1, F_OK)==-1 && 
		mknod(filename_1, S_IFIFO | 0666, 0) < 0)
	{
		exit(-1);
	}
	filename_1=argv[2];
	desc=ftok(filename_1, 45);
	semid=semget(desc, 0, S_IRUSR|S_IWUSR|IPC_CREAT);
	sb.sem_op=-1;
	sb.sem_flg=0;
	sb.sem_num=0;
	fd_out = open(filename_1, O_WRONLY);
	fd_in = fopen(argv[1], "r");
	if (fd_in == NULL){
		printf("Error: Cannot open file.\n");
		exit(-1);
	}
	args = malloc(MAX_ARGS * sizeof(char*));
	for(i = 0; i < MAX_ARGS; i++) args[i] = malloc(MAX_CHARS_ARG * sizeof(char));
	while ((read = getline(&line, &len, fd_in)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s\n", line);
		remove_newline(line);
		argsCount = split(line, args, ":");
		
		if(argsCount>2){
			printf("Error: Wrong input file format.\n");
			exit(-1);
		}
		int k;
		k=semctl(semid,0,GETVAL,0);
		printf("Waiting for sem, now %d\n", k);
		semop(semid, &sb, 1);
		printf("Sending %s to conveyor input\n", args[0]);
		ssize=strlen(args[0]);
		write(fd_out, &ssize, sizeof(int));
		write(fd_out,line,ssize*sizeof(char));
		ssize=atoi(args[1]);
		write(fd_out, &ssize, sizeof(int));
	}
	for(i = 0; i < MAX_ARGS; i++) free(args[i]);
	free(args);
	fclose(fd_in);
	close(fd_out);
	if (line) free(line);
	return 0;
}
