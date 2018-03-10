#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#define STRING_BUF_SIZE 256

void* receiveMessages(void *arg){
	char* input;
	int ssize;
	input=calloc(STRING_BUF_SIZE, sizeof(char*));
		while (1)
		{
			read(*((int*)arg), &ssize, sizeof(ssize));
			read(*((int*)arg), input, (ssize + 1)*sizeof(char));
			printf("re_1:%s", input);
		}
}
void sendMessages(int fd){
	int ssize = 1;
	char *input;
	input=calloc(STRING_BUF_SIZE, sizeof(char*));
	while (1)
	{
		fgets(input, STRING_BUF_SIZE-1, stdin);
		if(strlen(input) < 1) sleep(0.1);
		else{
			ssize=strlen(input);
			write(fd, &ssize, sizeof(ssize));
			write(fd,input,(strlen(input) + 1)*sizeof(char));
		}
				
	}
	free(input);


}
int main(int argc, char **argv){
	char *filename_1 = "/tmp/fifo_to.fifo";
	char *filename_2 = "/tmp/fifo_in.fifo";
	int err;
	if (access(filename_1, F_OK)==-1 && 
		mknod(filename_1, S_IFIFO | 0666, 0) < 0)
	{
		exit(-1);
	}

	if (access(filename_2, F_OK)==-1 && 
		mknod(filename_2, S_IFIFO | 0666, 0) < 0)
	{
		exit(-1);
	}
	int fd_in;
	fd_in= open(atoi(argv[1]) == 0 ? filename_2 : filename_1, 'r');
	pthread_t tid;
	err = pthread_create(&tid, NULL, &receiveMessages, &fd_in);
	if (err != 0){
		printf("\ncan't create thread :[%s]", strerror(err));
		exit(-1);
	}
	int fd_out;
	fd_out= open(atoi(argv[1]) == 0 ? filename_1 : filename_2, O_WRONLY);
	sendMessages(fd_out);
	pthread_join(tid, NULL);
	return 0;	
}
		
