#include <stdio.h>
#include <malloc.h>
#include "split.h"
#include <string.h>
int main(){
	char** res;
	int i, k;
	res=malloc(10*sizeof(char*));
	for (i=0; i<10; i++) res[i]=malloc(16*sizeof(char));
	char s[32];
	char del[8];
	fgets(s, 32*sizeof(char), stdin);
	s[strlen(s)-1]=0;
	fgets(del, 8*sizeof(char), stdin);
	del[strlen(del)-1]=0;
	k=split(s, res, del);
	for (i=0; i<k; i++) printf("%s\n", res[i]);
	return 0;
}
