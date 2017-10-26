#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
	assert(argc == 2);
	int n, m, s;
	sscanf(argv[1], "%d", &n);
	pid_t pid;
	m=n;
	foo: pid=fork();
	m--;
	if(pid==0){
		printf("Started %d\n", n-m);
		if(m) goto foo;

	}else{
		waitpid(pid, &s, 0);
		printf("Finished %d\n", n-m);
	}
	return 0;


}
