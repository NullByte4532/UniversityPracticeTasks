#include <cstdio>
#include "gamelogic.h"


int main(){
	Tree tree;
	FILE* file;
	char c, last=0;
	char ans[32];
	char q[64];
	file=fopen("tree.txt", "r");
	tree.load(file);
	fclose(file);
	while(true){
		if(last) printf("Is it ");
		printf("%s", tree.get());
		if(last) printf("?");
		printf("\n");
		c=getc(stdin);
		getc(stdin);
		if(c=='y'){
			if(!last)
			last=tree.step(YES);
			else{
			last=0;
			printf("YAY! try something else.\n");
			tree.rewind();	
			last=0;
			}
		}else{
			if(!last){
			last=tree.step(NO);
			}else{
				printf("Okay, you got me. What is it?\n");
				fgets(ans, 31, stdin);				
				ans[strlen(ans)-1]='\0';
				printf("What should I ask to tell the difference between %s and %s?\n", tree.get(), ans);
				fgets(q, 63, stdin);
				q[strlen(q)-1]='\0';
				printf("And what is the answer for %s?\n", tree.get());
				c=getc(stdin);
				getc(stdin);
				if(c=='y') tree.add(q, YES, ans);
				else tree.add(q, NO, ans);
				file=fopen("tree.txt", "w");
				tree.save(file);
				fclose(file);
				printf("I'll try to remember. Let's try again.\n");
				tree.rewind();
				last=0;
			}
		}
		
	}
return 0;	
}
