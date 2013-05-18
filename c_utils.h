#ifndef C_UTILS
#define C_UTILS

#include "basic_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct c_array
{
	byte* data;
	size_t elem_size;
	size_t len;
} c_array;

c_array init_c_array(byte* data, size_t elem_size, size_t len);
c_array copy_c_array(c_array src);


char* mystrdup(const char* str);

int file_open_read(const char* filename, const char* mode, c_array* out);
int file_read(FILE* file, c_array* out);

int file_open_readlines(const char* filename, c_array* lines, c_array* file_contents);
char* freadline(FILE* input);

char* freadstring(FILE* input, char delim, size_t max_len);

c_array slice_c_array(c_array array, int start, int end);


int split(c_array* in, byte* delim, size_t delim_len, c_array* out);

char* ltrim(char* str);
char* rtrim(char* str);
char* trim(char* str);

int any(c_array* array, int (*is_true)(byte*));
int all(c_array* array, int (*is_true)(byte*));

void map(c_array* array, void (*func)(byte*));


void boyermoore_search(c_array haystack_array, c_array needle_array);
void basic_search(c_array haystack, c_array needle);

/*
 * make string utility functions
 * split(char delim)
 * trim
 * etc
 * maybe also make versions that work on c_arrays/general types
 *
 * maybe copy/convert in my boyer-moore search function
 *
 *
 * also any(), all() and map()
 */





#ifdef __cplusplus
}
#endif

#endif
