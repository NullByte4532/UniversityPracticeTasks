#include <stdio.h>
#include <malloc.h>
#include "split.h"
#include <string.h>
int main(){
	char** result;
	int i, substringCount;
	result=malloc(10*sizeof(char*));
	for (i = 0; i < 10; i++) result[i] = malloc(16 * sizeof(char));
	char inputString[32];
	char delimiter[8];
	fgets(inputString, 32*sizeof(char), stdin);
	inputString[strlen(inputString)-1]=0;	//removing '\n'
	fgets(delimiter, 8*sizeof(char), stdin);
	delimiter[strlen(delimiter)-1]=0;	//removing '\n'
	substringCount=split(inputString, result, delimiter);
	for (i=0; i<substringCount; i++) printf("%s\n", result[i]);
	return 0;
}
