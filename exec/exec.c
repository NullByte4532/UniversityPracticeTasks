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
void remove_newline(char* string){
	if (string[strlen(string)-1]=='\n') string[strlen(string)-1]=0;
}
void run(FILE* input_file){
	char string[MAX_CHARS];
	char **args;
	int i, argsCount;
	args = malloc(MAX_ARGS * sizeof(char*));
	for(i = 0; i < MAX_ARGS; i++) args[i] = malloc(MAX_CHARS_ARG * sizeof(char));
	pid_t pid = 2;
	while(fgets(string, MAX_CHARS, input_file) != NULL){
		remove_newline(string);
		argsCount = split(string, args, " ");
		pid = fork();
		if(pid == 0){
			fclose(input_file);
			args[argsCount] = 0;
			execvp(args[0], args);
			for(i = 0; i < MAX_ARGS; i++) free(args[i]);
			free(args);
			exit(0);
			assert(0);
		}
		waitpid(pid, NULL, 0);
	}
	fclose(input_file);
	for(i = 0; i < MAX_ARGS; i++) free(args[i]);
	free(args);

}
int main(int argc, char *argv[]){
	int i;	
	if(argc == 1){
		run(stdin);
	}
	else{
		for(i = 1; i < argc; i++){
			run(fopen(argv[i],"r"));
		} 

	}

	return 0;
}
