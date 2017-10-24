#include <stdio.h>
#include <malloc.h>
#include <string.h>

size_t calculateRequiredMemory(int n){
	int tmp,i;
	tmp=0;
	for (i=0; i<n; i++)
	tmp=2*tmp+1;
	return (tmp+1)*sizeof(char);
}

void generateString(int n){
	int i;
	int l=0;
	char* s;
	char c=0;
	s=malloc(calculateRequiredMemory(n));
	s[0]=0;
	for (i=0; i<n; i++){
		s[l]='a'+c%26;
		c++;
		if(l!=0)
		strncpy(s+l+1, s, l*sizeof(char));		
		l=l*2+1;
		s[l]=0;
		printf("%s\n", s);
	}

}
int main(){
int n;
scanf("%d", &n);
generateString(n);
return 0;
}
