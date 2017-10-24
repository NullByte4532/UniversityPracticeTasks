#include <string.h>
#include "split.h"
int split(char* src, char** dst, char* del){
	if (!(src&&dst&&del)) return -1;
	int n=0, i=0, j=0, k=0;
	int l;
	l=strlen(del);
	while (src[i]!=0){
		if (src[i]==del[j]){
			j++;
			if(j==l){
				j=0;
				dst[n][k]=0;
				n++;
				k=0;
			}
		}else{
			if(j){
				strncpy(dst[n], del, j);
				k=k+j;
				j=0;
				
			}
			dst[n][k]=src[i];
			k++;
			
		
		}
		i++;
	}

	dst[n][k]=0;
	n++;

	return n;
}
