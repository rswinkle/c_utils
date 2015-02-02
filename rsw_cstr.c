#include "rsw_cstr.h"

#include <assert.h>

#define STDERR(X) fprintf(stderr, X)

#define RSW_CSTR_ALLOCATOR(x) ((x) * 2)


size_t CSTR_ST_SZ = 25;


//vector_char* vec_char_heap(size_t size, size_t capacity);
//vector_char* init_vec_char_heap(char* vals, size_t num);



int init_cstr(rsw_cstr* str, size_t size, size_t capacity)
{
	str->size = size;
	str->capacity = (capacity > str->size) ? capacity : str->size + CSTR_ST_SZ;

	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}

	return 1;
}


int init_cstr_str(cstr* str, char* start, size_t num)
{
	str->capacity = num + CSTR_ST_SZ;
	str->size = num;
	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}

	memcpy(str->a, vals, sizeof(char)*num);
	str->a[num] = 0;

	return 1;
}

void cstr_copy(void* dest, void* src)
{
	rsw_cstr* str1 = (rsw_cstr*)dest;
	rsw_cstr* str2 = (rsw_cstr*)src;
	
	char* tmp;
	if (str1->capacity < str2->size + 1) {
		/*not much else we can do here*/
		if (!(tmp = (char*)realloc(str1->a, str2->capacity*sizeof(char)))) {
			assert(tmp != NULL);
			return;
		}
		str1->a = tmp;
		str1->capacity = str2->capacity;
	}
	
	memcpy(str1->a, str2->a, str2->size*sizeof(char));
	str1->a[str1->size] = 0;
	str1->size = str2->size;
}


int cstr_push(rsw_cstr* str, char a)
{
	char* tmp;
	size_t tmp_sz;
	if (str->capacity > str->size + 1) {
		str->a[str->size++] = a;
		str->a[str->size] = 0;
	} else {
		tmp_sz = RSW_CSTR_ALLOCATOR(str->capacity);
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		str->a[str->size++] = a;
		str->a[str->size] = 0;
		str->capacity = tmp_sz;
	}
	return 1;
}



char cstr_pop(rsw_cstr* str)
{
	char tmp = str->a[--str->size];
	str->a[str->size] = 0;
	return tmp;
}


int cstr_extend(rsw_cstr* str, size_t num, char a)
{
	char* tmp;
	size_t tmp_sz;
	if (str->capacity < str->size + num + 1) {
		tmp_sz = str->capacity + num + CSTR_ST_SZ;
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		str->capacity = tmp_sz;
	}

	memset(&str->a[str->size], a, num);
	str->size += num;
	str->a[str->size] = 0;
	return 1;
}


int cstr_insert(rsw_cstr* str, size_t i, char a)
{
	char* tmp;
	size_t tmp_sz;
	if (str->capacity > str->size + 1) {
		memmove(&str->a[i+1], &str->a[i], (str->size-i)*sizeof(char));
		str->a[i] = a;
	} else {
		tmp_sz = RSW_CSTR_ALLOCATOR(str->capacity);
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		memmove(&str->a[i+1], &str->a[i], (str->size-i)*sizeof(char));
		str->a[i] = a;
		str->capacity = tmp_sz;
	}

	str->size++;
	str->a[str->size] = 0;
	return 1;
}



int cstr_insert_str(rsw_cstr* str, size_t i, char* a, size_t num);
{
	char* tmp;
	size_t tmp_sz;
	if (str->capacity < str->size + num + 1) {
		tmp_sz = str->capacity + num + CSTR_ST_SZ;
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		str->capacity = tmp_sz;
	}

	memmove(&str->a[i+num], &str->a[i], (str->size-i)*sizeof(char));
	memcpy(&str->a[i], a, num*sizeof(char));
	str->size += num;
	str->a[str->size] = 0;
	return 1;
}

int cstr_insert_cstr(rsw_cstr* str, size_t i, rsw_cstr* a_str)
{
	char* tmp;
	size_t tmp_sz;
	size_t num = a_str->size;
	if (str->capacity < str->size + num + 1) {
		tmp_sz = str->capacity + num + CSTR_ST_SZ;
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		str->capacity = tmp_sz;
	}

	memmove(&str->a[i+num], &str->a[i], (str->size-i)*sizeof(char));
	memcpy(&str->a[i], a, num*sizeof(char));
	str->size += num;
	str->a[str->size] = 0;
	return 1;
}

void cstr_erase(rsw_cstr* str, size_t start, size_t end)
{
	size_t d = end - start + 1;
	memmove(&str->a[start], &str->a[end+1], (str->size-1-end)*sizeof(char));
	str->size -= d;
	str->a[str->size] = 0;
}


int cstr_reserve(rsw_cstr* str, size_t size)
{
	char* tmp;
	if (str->capacity < size + 1) {
		if (!(tmp = (char*)realloc(str->a, sizeof(char)*(size+CSTR_ST_SZ)))) {
			assert(tmp != NULL);
			return 0;
		}
		str->a = tmp;
		str->capacity = size + CSTR_ST_SZ;
	}
	return 1;
}




int cstr_set_capacity(rsw_cstr* str, size_t size)
{
	char* tmp;
	if (size < str->size) {
		str->size = size;
	}

	if (!(tmp = (char*)realloc(str->a, sizeof(char)*(size+1)))) {
		assert(tmp != NULL);
		return 0;
	}
	str->a = tmp;
	str->capacity = size+1;
	str->a[str->size] = 0;
	return 1;
}



void cstr_set_val_sz(rsw_cstr* str, char val)
{
	size_t i;
	for (i=0; i<str->size; i++) {
		str->a[i] = val;
	}
}

//not sure when I'd use this
void cstr_set_val_cap(rsw_cstr* str, char val)
{
	size_t i;
	for (i=0; i<str->capacity-1; i++) {
		str->a[i] = val;
	}
	str->a[str->capacity-1] = 0;
}


void cstr_clear(rsw_cstr* str) { str->size = 0; }

void free_cstr_heap(void* str)
{
	rsw_cstr* tmp = (rsw_cstr*)str;
	free(tmp->a);
	free(tmp);
}

void free_cstr(void* str)
{
	rsw_cstr* tmp = (rsw_cstr*)str;
	free(tmp->a);
	tmp->size = 0;
	tmp->capacity = 0;
}

