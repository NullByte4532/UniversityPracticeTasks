#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define STRING_BUF_SIZE 256
int main(int argc, char **argv){
	char *input;
	char *filename_1 = "/tmp/fifo_to.fifo";
	char *filename_2 = "/tmp/fifo_in.fifo";
	int ssize = 1;
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
		pid_t pid = fork();
		if (pid == 0)
		{	input=calloc(STRING_BUF_SIZE, sizeof(char*));
			int fd_1;
			fd_1= open(atoi(argv[1]) == 0 ? filename_1 : filename_2, O_WRONLY);
			while (1)
			{
				fgets(input, STRING_BUF_SIZE-1, stdin);
				if(strlen(input) < 1) sleep(0.1);
				else{
					ssize=strlen(input);
					write(fd_1, &ssize, sizeof(ssize));
					write(fd_1,input,(strlen(input) + 1)*sizeof(char));
				}
				
			}
		}else{
			int fd_2;
			fd_2= open(atoi(argv[1]) == 0 ? filename_2 : filename_1, 'r');
			input=calloc(STRING_BUF_SIZE, sizeof(char*));
			while (1)
			{
				read(fd_2, &ssize, sizeof(ssize));
				read(fd_2, input, (ssize + 1)*sizeof(char));
				printf("re_1:%s", input);
			}
		}
	return 0;	
}
		
