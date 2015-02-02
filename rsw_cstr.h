#ifndef RSW_CSTR_H
#define RSW_CSTR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rsw_cstr
{
	char* a;           /**< Array. */
	size_t size;       /**< current size, ie what strlen() would return or std::string::size() */
	size_t capacity;   /**< Allocated size of array; always >= size + 1 (for '\0'). */
} rsw_cstr;



extern size_t CSTR_ST_SZ;

int init_cstr(rsw_cstr* str, size_t size, size_t capacity);
int init_cstr_str(cstr* str, char* start, size_t num);

//rsw_cstr* vec_char_heap(size_t size, size_t capacity);
//rsw_cstr* init_vec_char_heap(char* vals, size_t num);

void cstr_copy(void* dest, void* src);

int cstr_push(rsw_cstr* str, char a);
char cstr_pop(rsw_cstr* str);

int cstr_extend(rsw_cstr* str, size_t num, char a);
int cstr_insert(rsw_cstr* str, size_t i, char a);

int cstr_insert_str(rsw_cstr* str, size_t i, char* a, size_t num);
int cstr_insert_cstr(rsw_cstr* str, size_t i, rsw_cstr* a_str);

void cstr_erase(rsw_cstr* str, size_t start, size_t end);

int cstr_reserve(rsw_cstr* str, size_t size);
int cstr_set_capacity(rsw_cstr* str, size_t size);

void cstr_set_val_sz(rsw_cstr* str, char val);
void cstr_set_val_cap(rsw_cstr* str, char val);


void cstr_clear(rsw_cstr* str);
void free_cstr_heap(void* str);
void free_cstr(void* str);

#ifdef __cplusplus
}
#endif

#endif


