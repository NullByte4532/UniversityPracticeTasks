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
#include <sys/sem.h>
#include "config.h"
int main(int argc, char **argv){
	char * line = NULL;
	int i, num, ssize, time;
	int fd_in;
	int desc;
	int semid;
	struct sembuf sb;
	if (argc!=3){
		printf("Error: Wrong number of arguments\n");
		exit(-1);
	}
	if (access(argv[1], F_OK)==-1 && 
		mknod(argv[1], S_IFIFO | 0666, 0) < 0)
	{
		printf("Error: No permission to create or access FIFO.\n");
		exit(-1);
	}
	desc=ftok(argv[1], 45);
	semid=semget(desc, 0, S_IRUSR|S_IWUSR|IPC_CREAT);
	sb.sem_op=1;
	sb.sem_flg=0;
	sb.sem_num=0;
	if (semctl(semid, 0, SETVAL, atoi(argv[2])) == -1) {
		perror("Error: semctl failed");
		exit(-1);
	};
	fd_in= open(argv[1], 'r');
	line=calloc(MAX_CHARS_ARG, sizeof(char*));
	while (1) {
		sb.sem_op=1;
		semop(semid, &sb, 1);
		read(fd_in, &ssize, sizeof(ssize));
		read(fd_in, line, (ssize)*sizeof(char));
		read(fd_in, &num, sizeof(int));
		line[ssize]=0;
		if(!strcmp(line, "exit")){
			break;
		}
		printf("Outputing %d of %s.\n", num, line);	
	}
	if (line) free(line);
	close(fd_in);
	return 0;
}
