#include <string.h>
#include "split.h"
int split(char* source, char** destination, char* delimiter){
	if (!(source && destination && delimiter)) return -1;
	int substring_number = 0, source_index = 0, delimiter_index = 0, substring_index = 0;
	int delimiterLength;
	delimiterLength = strlen(delimiter);
	while (source[source_index] != 0){
		if (source[source_index] == delimiter[delimiter_index]){
			delimiter_index++;
			if(delimiter_index == delimiterLength){
				delimiter_index = 0;
				destination[substring_number][substring_index] = 0;
				substring_number++;
				substring_index = 0;
			}
		}else{
			if(j){
				strncpy(destination[substring_number], delimiter, delimiter_index);
				substring_index = substring_index + delimiter_index;
				delimiter_index = 0;
				
			}
			destination[substring_number][substring_index] = source[source_index];
			substring_index++;
			
		
		}
		source_index++;
	}

	destination[substring_number][substring_index] = 0;
	substring_number++;

	return substring_number;
}
