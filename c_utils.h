#ifndef C_UTILS
#define C_UTILS

#include "basic_types.h"
#include <stdio.h>


typedef struct
{
	byte* data;
	size_t len;
} c_array;


int file_open_read(const char* filename, const char* mode, c_array* out);

int file_read(FILE* file, c_array* out);

char* freadstring(FILE* input, char delim, size_t max_len);

c_array slice_c_array(c_array array, int start, int end);


#endif
