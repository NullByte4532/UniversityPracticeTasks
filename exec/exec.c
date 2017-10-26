#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include "../split/split.h"
#define MAX_ARGS 16
#define MAX_CHARS 32
#define MAX_CHARS_ARG 16
void remove_newline(char* s){
	if (s[strlen(s)-1]=='\n') s[strlen(s)-1]=0;
}
void run(FILE* f){
	char s[MAX_CHARS];
	char **a;
	int i, n;
	a=malloc(MAX_ARGS*sizeof(char*));
	for(i=0; i<MAX_ARGS; i++) a[i]=malloc(MAX_CHARS_ARG*sizeof(char));
	pid_t p=2;
	while(fgets(s, MAX_CHARS, f)!=NULL){
		remove_newline(s);
		n=split(s, a, " ");
		p=fork();
		if(p==0){
			fclose(f);
			a[n]=0;
			execvp(a[0], a);
			for(i=0; i<MAX_ARGS; i++) free(a[i]);
			free(a);
			exit(0);
			assert(0);
		}
		waitpid(p, &n, 0);
	}
	fclose(f);
	for(i=0; i<MAX_ARGS; i++) free(a[i]);
	free(a);

}
int main(int argc, char *argv[]){
	int i;	
	if(argc==1){
		run(stdin);
	}
	else{
		for(i=1; i<argc; i++){
			run(fopen(argv[i],"r"));
		} 

	}

	return 0;
}
