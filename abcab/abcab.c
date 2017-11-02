#include <stdio.h>
#include <malloc.h>
#include <string.h>

size_t calculateRequiredMemory(int lineCount){
	int char_count, i;
	char_count = 0;
	for (i = 0; i < lineCount; i++)
	char_count = 2 * char_count + 1;
	return (char_count + 1) * sizeof(char);
}

void generateString(int lineCount){
	int i;
	int length = 0;
	char* str;
	char letter_num = 0;
	str = malloc(calculateRequiredMemory(lineCount));
	str[0] = 0;
	for (i = 0; i < lineCount; i++){
		str[length] = 'a' + letter_num % 26;
		letter_num++;
		if(length != 0)
		strncpy(str + length + 1, str, length * sizeof(char));		
		length = length * 2 + 1;
		str[length] = 0;
		printf("%s\n", str);
	}

}
int main(){
	int lineCount;
	scanf("%d", &lineCount);
	generateString(lineCount);
	return 0;
}
