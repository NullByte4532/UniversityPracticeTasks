#include <stdio.h>
typedef char (*cmpfunc)(int, int);
void sort(int* ar, int len, cmpfunc cmp ){
int i, j;
int k, t;
for (i=0; i<len; i++){
	k=i;
	for (j=i; j<len; j++){
		if(cmp(ar[k], ar[j])>0) k=j;
	}
	t=ar[i];
	ar[i]=ar[k];
	ar[k]=t;
}

}
char c(int a, int b){
if(a>b) return 1;
else return -1;

}
char d(int a, int b){
if(a>b) return -1;
else return 1;

}
int main(){
int lol[4]={3,7,1,56};
int i;
sort(lol, 4, c);
for (i=0; i<4; i++) printf("%d\n", lol[i]);
sort(lol, 4, d);
printf("==========================\n");
for (i=0; i<4; i++) printf("%d\n", lol[i]);

return 0;
}
