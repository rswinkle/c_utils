#include "c_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int file_open_read(const char* filename, const char* mode, c_array* out)
{
	int tmp;
	byte* data;
	FILE *file;
	size_t size;
	out->data = NULL;
	out->len = 0;
	
	file = fopen(filename, mode);
	if (!file)
		return 0;
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	data = malloc(size);
	if (!data) {
		fclose(file);
		return 0;
	}
		
	rewind(file);
	
	if (!fread(data, size, 1, file)) {
		printf("read failure\n");
		fclose(file);
		free(data);
		return 0;
	}
	
	data[size] = 0; /* null terminate in all cases even if reading binary data */
	
	out->data = data;
	out->len = size;
	
	fclose(file);
	return size;
}



int file_read(FILE* file, c_array* out)
{
	int tmp;
	byte* data;
	size_t size;
	out->data = NULL;
	out->len = 0;
	
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	data = malloc(size);
	if (!data) {
		fclose(file);
		return 0;
	}
		
	rewind(file);
	
	if ((tmp = fread(data, sizeof(char), size, file)) != size) {
		printf("read failure\n");
		fclose(file);
		free(data);
		return 0;
	}
	
	data[size] = 0; /* null terminate in all cases even if reading binary data */
	
	out->data = data;
	out->len = size;
	
	fclose(file);
	return size;
}


char* freadstring(FILE* input, char delim, size_t max_len);
{
	char* string = malloc(300*sizeof(char));
	char temp;
	int i=0;
	for(i; i<max_len; i++) {
		temp = fgetc(input);

		if (temp == EOF || temp == delim) {
			string[i] = '\0';
			string = realloc(string,(i+1)*sizeof(char));
			break;
		}
		string[i] = temp;
	}
	if (i == max_len) string[i] = '\0';


	return string;
}



c_array slice_c_array(c_array array, int start, int end)
{
	c_array a;
	int len;
	
	a.data = NULL;
	a.len = 0;
	
	if (start < 0)
		start = array.len + start;
	
	if (end < 0)
		end = array.len + end;
	
	if (start < 0) 
		start = 0;
	
	if (end < 0)
		end = 0;
	
	if (end < start)
		return a;
	
	len = end - start;
	if (!(a.data = malloc(len)))
		return a;
	
	memcpy(a.data, &array.data[start], len);
	a.len = len;
	return a;
}
	
	
	
	








