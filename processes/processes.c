#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
	assert(argc == 2);
	int processCount, processCountRemaining, stat;
	sscanf(argv[1], "%d", &processCount);
	pid_t pid;
	processCountRemaining = processCount;
	foo: pid = fork();
	processCountRemaining--;
	if(pid == 0){
		printf("Started %d\n", processCount - processCountRemaining);
		if(processCountRemaining) goto foo;

	}else{
		waitpid(pid, &stat, 0);
		printf("Finished %d with status %d\n", processCount - processCountRemaining, stat);
	}
	return 0;


}
