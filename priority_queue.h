#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>
#include <assert.h>

typedef struct heap_t
{
	unsigned char* a;
	size_t size;
	size_t capacity;
	size_t elem_size;
	int(*cmp)(const void*, const void*);
} heap_t;


void maxheapify(int* a, size_t i, size_t heapsize);
void build_maxheap(int* a, size_t n);
int maxheap_pop(int* a, size_t* heapsize);
int maxheap_inc_key(int* a, size_t heapsize, size_t i, int key);
int maxheap_pushe(int* a, size_t* heapsize, size_t* cap, int key);
int maxheap_push(int* a, size_t* heapsize, size_t cap, int key);


void heapify(void* a, size_t n, size_t size, int(*compare)(const void*, const void*));
void heappop(void* a, size_t n, size_t size, int(*compare)(const void*, const void*));



/*

#define declare_priority_queue(FUNCNAME, TYPE) \
	void FUNCNAME(TYPE* a, size_t i, size_t n)

#define define_priority_queue(FUNCNAME, TYPE, COMPARE) \
                                                                     \
void FUNCNAME_heapify(TYPE* a, size_t i, size_t n)                           \
{                                                                    \
	TYPE* largest, *p1, *p2, *last;                                  \
	TYPE temp;                                                       \
	size_t p1, p2;                                                   \
	                                                                 \
	p1 = 2*i+1;                                                      \
	p2 = i;                                                          \
	if (p1 < heapsize && COMPARE) {                                  \
		p2 = p1;                                                     \
	}                                                                \
                                                                     \
	++p1;                                                            \
	if (p1 <= last && COMPARE) {                                     \
		p2 = p1;                                                     \
	}                                                                \
                                                                     \
	if (p2 != i) {                                                   \
		temp = a[i];                                                 \
		a[i] = a[p2];                                                \
		a[p2] = temp;                                                \
		FUNCNAME(a, p2, heapsize);                                   \
	}                                                                \
}


*/

#endif

#ifdef PRIORITY_QUEUE_IMPLEMENTATION

#define parent(x) (((x)-1)/2)

void maxheapify(int* a, size_t i, size_t heapsize)
{
	size_t largest;
	int temp;

	size_t l = 2*i + 1;
	size_t r = l + 1;
	largest = i;
	if (l <= heapsize && a[l] > a[i]) {
		largest = l;
	}

	if (r <= heapsize && a[r] > a[largest]) {
		largest = r;
	}

	if (largest != i) {
		temp = a[i];
		a[i] = a[largest];
		a[largest] = temp;
		maxheapify(a, largest, heapsize);
	}
	return;
}

void build_maxheap(int* a, size_t n)
{
	size_t i;
	for (i=(n-2)/2; i>0; --i) {
		maxheapify(a, i, n-1);
	}
	maxheapify(a, i, n-1);
}

int maxheap_pop(int* a, size_t* heapsize)
{
	if (!*heapsize) {
		assert(*heapsize);
		return 0;
	}
	int max = a[0];
	a[0] = a[--(*heapsize)];
	maxheapify(a, 0, *heapsize);
	return max;
}

int maxheap_inc_key(int* a, size_t heapsize, size_t i, int key)
{
	if (key < a[i]) {
		assert(key >= a[i]);
		return 0;
	}
	a[i] = key;
	int tmp;
	while (i && a[parent(i)] < a[i]) {
		tmp = a[i];
		a[i] = a[parent(i)];
		a[parent(i)] = tmp;
		i = parent(i);
	}
	return 1;
}

int maxheap_push(int* a, size_t* heapsize, size_t cap, int key)
{
	int tmp;
	if (cap == *heapsize) {
		assert(cap > *heapsize);
		return 0;
	}
	a[*heapsize] = INT_MIN;
	if (!maxheap_inc_key(a, *heapsize+1, *heapsize, key))
		return 0;

	(*heapsize)++;
	return 1;
}

int maxheap_pushe(int* a, size_t* heapsize, size_t* cap, int key)
{
	int* tmp;
	size_t tmp_sz;
	if (*cap == *heapsize) {
		tmp_sz = *cap * 2;
		if (!(tmp = (int*)realloc(a, sizeof(int)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		a = tmp;
		*cap = tmp_sz;
	}
	a[*heapsize] = INT_MIN;
	maxheap_inc_key(a, *heapsize+1, *heapsize, key);
	(*heapsize)++;
	return 1;
}
	
#endif
