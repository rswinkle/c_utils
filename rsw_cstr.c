#include "rsw_cstr.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define STDERR(X) fprintf(stderr, X)

#define RSW_CSTR_ALLOCATOR(x) ((x) * 2)


size_t CSTR_ST_SZ = 25;


//vector_char* vec_char_heap(size_t size, size_t capacity);
//vector_char* init_vec_char_heap(char* vals, size_t num);

int init_cstr(rsw_cstr* str)
{
	str->size = 0;
	str->capacity = CSTR_ST_SZ;

	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}
	str->a[0] = 0;

	return 1;
}

int init_cstr_cap(rsw_cstr* str, size_t capacity)
{
	str->size = 0;
	str->capacity = (capacity > 0) ? capacity : CSTR_ST_SZ;

	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->capacity = 0;
		return 0;
	}

	return 1;
}

int init_cstr_sz(rsw_cstr* str, size_t size, char val)
{
	str->size = size;
	str->capacity = size + CSTR_ST_SZ;

	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}

	for (int i=0; i<size; ++i)
		str->a[i] = val;
	str->a[size] = 0;

	return 1;
}

int init_cstr_sz_cap(rsw_cstr* str, size_t size, char val, size_t capacity)
{
	str->size = size;
	str->capacity = (capacity > size) ? capacity : size + CSTR_ST_SZ;

	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}

	for (int i=0; i<size; ++i)
		str->a[i] = val;
	str->a[size] = 0;

	return 1;
}


int init_cstr_str(rsw_cstr* str, char* start, size_t num)
{
	str->capacity = num + CSTR_ST_SZ;
	str->size = num;
	if (!(str->a = (char*)malloc(str->capacity*sizeof(char)))) {
		assert(str->a != NULL);
		str->size = str->capacity = 0;
		return 0;
	}

	memcpy(str->a, start, sizeof(char)*num);
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



int cstr_insert_str(rsw_cstr* str, size_t i, char* a, size_t num)
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
	memcpy(&str->a[i], a_str->a, num*sizeof(char));
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

/* Sets a cstr to first num chars in a (clears previous contents) */
int cstr_set_str(rsw_cstr* str, char* a, size_t num)
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

	memcpy(str->a, a, num*sizeof(char));
	str->size = num;
	str->a[str->size] = 0;
	return 1;
}

rsw_cstr* cstr_ltrim(rsw_cstr* str)
{
	size_t i = 0;

	while (isspace(str->a[i++]));

	memmove(str->a, &str->a[i], str->size-i);
	str->size -= i;
	str->a[str->size] = 0;

	return str;
}

rsw_cstr* cstr_rtrim(rsw_cstr* str)
{
	size_t i;

	i = str->size - 1;
	while (i != (size_t)-1 && isspace(str->a[i--]));

	str->size = i + 1;
	str->a[str->size] = 0;
	return str;
}

rsw_cstr* cstr_trim(rsw_cstr* str)
{
	return cstr_ltrim(cstr_rtrim(str));
}

/* returns a new string like python.  I'll have to check
 * the return string's members to know if it failed or not */
rsw_cstr slice_cstr(rsw_cstr* str, long start, long end)
{
	rsw_cstr str2 = { NULL, 0, 0 };
	int len;

	if (start < 0)
		start = str->size + start;

	if (end < 0)
		end = str->size + end;

	if (start < 0)
		start = 0;

	if (end < 0)
		end = 0;

	if (end < start)
		end = start;

	len = end - start;

	init_cstr_str(&str2, str->a, len);
	return str2;
}

rsw_cstr cstr_substr(rsw_cstr* str, size_t index, size_t len)
{
	rsw_cstr s;

	assert(index < str->size);

	len = (len > str->size - index) ? str->size-index : len;

	init_cstr_str(&s, &str->a[index], len);
	return s;
}

int cstr_resize(rsw_cstr* str, size_t size, char val)
{
	if (!cstr_reserve(str, size))
		return 0;

	size_t i;
	if (size > str->size)
		for (i=str->size; i<size; ++i) {
			str->a[i] = val;
		}

	str->size = size;
	str->a[size] = 0;
	return 1;
}

/* probably just use strstr directly most of the time
 * but I'm trying to parallel C++ std::string functions */
size_t cstr_find(rsw_cstr* str, rsw_cstr* needle)
{
	char* result = strstr(str->a, needle->a);
	if (result)
		return result - str->a;
	else
		return -1; /* TODO make a macro or static const size_t npos = -1 ? */
}

size_t cstr_find_start_at(rsw_cstr* str, rsw_cstr* needle, size_t start)
{
	assert(start < str->size);

	char* result = strstr(&str->a[start], needle->a);
	if (result)
		return result - str->a;
	else
		return -1; /* TODO make a macro or static const size_t npos = -1 ? */
}


int cstr_replace(rsw_cstr* str, size_t index, size_t num, rsw_cstr* str2)
{
	assert(index < str->size);

	size_t len = (num < str2->size) ? num : str2->size;

	if (!cstr_reserve(str, index + len)) {
		return 0;
	}

	for (size_t i=0; i<len; ++i) {
		str->a[index+i] = str2->a[i];
	}

	return 1;
}

int cstr_split(rsw_cstr* str, rsw_cstr* delim, rsw_cstr** results, size_t* num_results)
{
	if (!results || !num_results)
		return 0;

	size_t num = 0;
	
	rsw_cstr* ret, *tmp;
	if (delim->size == 0) {
		if (!(ret = (rsw_cstr*)malloc(sizeof(rsw_cstr)))) {
			assert(ret);
			return 0;
		}
		num = 1;
		init_cstr_str(ret, str->a, str->size);
		return 1;
	}

	int cap = 10;
	ret = (rsw_cstr*)malloc(sizeof(rsw_cstr)*cap);
	
	char* search = str->a;
	char* result;
	while (result = strstr(search, delim->a)) {
		init_cstr_str(&ret[num], search, result-search);
		++num;

		//since there's
		if (num == cap) {
			if (!(tmp = (rsw_cstr*) realloc(ret, sizeof(rsw_cstr)*cap*2))) {
				assert(tmp);
				free(ret);
				num = 0;
				return 0;
			}
			ret = tmp;
			cap *= 2;
		}
		search = result + delim->size;
	}

	//if the last thing in the string was the delimiter then search will be '\0'
	if (search) {
		init_cstr_str(&ret[num], search, str->a+str->size-search);
		++num;
	}

	if (!(tmp = (rsw_cstr*) realloc(ret, sizeof(rsw_cstr)*num))) {
		assert(tmp);
		free(ret);
		num = 0;
		return 0;
	}

	*num_results = num;
	*results = tmp;
	return 1;
}


