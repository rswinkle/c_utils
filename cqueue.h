#ifndef CQUEUE_H
#define CQUEUE_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>


#ifdef __cplusplus
extern "C" {
#endif

#define QUE_READ  0
#define QUE_WRITE 1

typedef struct queue_i
{
	size_t head;
	size_t tail;

	int* buf;
	size_t capacity;
	int lastop;
} queue_i;

extern size_t QUE_I_START_CAP;

int que_i(queue_i* q, size_t capacity);
int que_push_i(queue_i* q, int a);
int que_pop_i(queue_i* q);

int que_pushe_i(queue_i* q, int a);
int que_resize_i(queue_i* q, size_t size);

int que_front_i(queue_i* q);
int que_back_i(queue_i* q);
int que_is_empty_i(queue_i* q);
int que_is_full_i(queue_i* q);
size_t que_size_i(queue_i* q);

void free_que_i(void* q);


typedef struct queue_d
{
	size_t head;
	size_t tail;

	double* buf;
	size_t capacity;
	int lastop;
} queue_d;

extern size_t QUE_D_START_CAP;

int que_d(queue_d* q, size_t capacity);
int que_push_d(queue_d* q, double a);
double que_pop_d(queue_d* q);

int que_pushe_d(queue_d* q, double a);
int que_resize_d(queue_d* q, size_t size);

double que_front_d(queue_d* q);
double que_back_d(queue_d* q);
int que_is_empty_d(queue_d* q);
int que_is_full_d(queue_d* q);
size_t que_size_d(queue_d* q);

void free_que_d(void* q);



typedef struct queue_str
{
	size_t head;
	size_t tail;

	char** buf;
	size_t capacity;
	int lastop;
} queue_str;

#define QUE_READ  0
#define QUE_WRITE 1

extern size_t QUE_STR_START_CAP;

/* hmm */
char* mystrdup(const char* str);

int que_str(queue_str* q, size_t capacity);
int que_push_str(queue_str* q, char* a);
void que_pop_str(queue_str* q);

int que_pushe_str(queue_str* q, char* a);
int que_resize_str(queue_str* q, size_t size);

char* que_front_str(queue_str* q);
char* que_back_str(queue_str* q);
int que_is_empty_str(queue_str* q);
int que_is_full_str(queue_str* q);
size_t que_size_str(queue_str* q);

void free_que_str(void* q);


typedef unsigned char byte;

typedef struct queue_void
{
	size_t head;
	size_t tail;

	byte* buf;
	size_t capacity;
	int lastop;

	size_t elem_size;
	void (*elem_init)(void*, void*);
	void (*elem_free)(void*);

} queue_void;

#define QUE_READ  0
#define QUE_WRITE 1

extern size_t QUE_VOID_START_CAP;

#define GET_VOID(QUE, TYPE, I) ((TYPE*)&(QUE)->a[(I)*(QUE)->elem_size])

int que_void(queue_void* q, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*));
int que_push_void(queue_void* q, void* a);
void que_pop_void(queue_void* q);

int que_pushe_void(queue_void* q, void* a);
int que_resize_void(queue_void* q, size_t size);

void* que_front_void(queue_void* q);
void* que_back_void(queue_void* q);
int que_is_empty_void(queue_void* q);
int que_is_full_void(queue_void* q);
size_t que_size_void(queue_void* q);

void free_que_void(void* q);


#ifdef __cplusplus
}
#endif


/* header ends */
#endif

#ifdef CQUEUE_IMPLEMENTATION

size_t QUE_I_START_CAP = 50;

#define QUE_I_ALLOCATOR(x) ((x) * 2)

int que_i(queue_i* q, size_t capacity)
{
	q->head = q->tail = 0;
	q->lastop = QUE_READ;
	q->capacity = capacity ? capacity : QUE_I_START_CAP;

	if (!(q->buf = (int*)malloc(q->capacity*sizeof(int)))) {
		assert(q->buf != NULL);
		q->capacity = 0;
		return 0;
	}

	return 1;
}

int que_push_i(queue_i* q, int a)
{
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		assert(q->head != q->tail || q->lastop != QUE_WRITE);
		return 0;
	}

	q->buf[q->tail] = a;
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

int que_pushe_i(queue_i* q, int a)
{
	int* tmp;
	size_t tmp_sz, inc;
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		tmp_sz = QUE_I_ALLOCATOR(q->capacity);
		inc = tmp_sz - q->capacity;
		if (!(tmp = (int*)realloc(q->buf, sizeof(int)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

		/* hmmm */
		if (q->head) {
			memmove(&q->buf[q->head+inc], &q->buf[q->head], (q->capacity-q->head)*sizeof(int));
		} else {
			q->tail = q->capacity;
		}

		q->capacity = tmp_sz;
	}

	q->buf[q->tail] = a;
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

/*
 * like vector pop_i, it's programmers responsibility 
 * to make sure it's not empty
*/
int que_pop_i(queue_i* q)
{
	int a;

	assert(q->head != q->tail || q->lastop == QUE_WRITE);

	a = q->buf[q->head];
	q->head++;
	q->head %= q->capacity;
	q->lastop = QUE_READ;

	if (q->head == q->tail)
		q->head = q->tail = 0;

	return a;
}

int que_resize_i(queue_i* q, size_t size)
{
	size_t sz;
	int* tmp = NULL;

	sz = que_size_i(q);
	if (size < sz) {
		assert(size >= sz);
		return 0;
	}

	if (size > q->capacity && q->tail > q->head) {
		if (!(tmp = (int*)realloc(q->buf, sizeof(int)*size))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

	} else {
		tmp = (int*)malloc(size*sizeof(int));
		if (q->tail <= q->head) {
			memcpy(tmp, &q->buf[q->head], (q->capacity-q->head)*sizeof(int));
			memcpy(&tmp[q->capacity-q->head], q->buf, q->tail*sizeof(int));
		} else {
			memcpy(tmp, &q->buf[q->head], (q->tail-q->head)*sizeof(int));
		}
		free(q->buf);
		q->buf = tmp;
		q->head = 0;
		q->tail = sz;
	}

	q->capacity = size;
	return 1;
}

int que_front_i(queue_i* q)
{
	return q->buf[q->head];
}

int que_back_i(queue_i* q)
{
	return q->buf[q->tail-1];
}


int que_is_empty_i(queue_i* q)
{
	return q->head == q->tail && q->lastop == QUE_READ;
}

int que_is_full_i(queue_i* q)
{
	return q->head == q->tail && q->lastop == QUE_WRITE;
}

size_t que_size_i(queue_i* q)
{
	if (q->tail < q->head) {
		return q->capacity - q->head + q->tail;
	} else if (q->tail > q->head) {
		return q->tail - q->head;
	} else if (q->lastop == QUE_WRITE) {
		return q->capacity;
	} else {
		return 0;
	}
}

void free_que_i(void* q)
{
	queue_i* tmp = (queue_i*)q;
	free(tmp->buf);
	tmp->capacity = 0;
}


size_t QUE_D_START_CAP = 50;

#define QUE_D_ALLOCATOR(x) ((x) * 2)

int que_d(queue_d* q, size_t capacity)
{
	q->head = q->tail = 0;
	q->lastop = QUE_READ;
	q->capacity = capacity ? capacity : QUE_D_START_CAP;

	if (!(q->buf = (double*)malloc(q->capacity*sizeof(double)))) {
		assert(q->buf != NULL);
		q->capacity = 0;
		return 0;
	}

	return 1;
}

int que_push_d(queue_d* q, double a)
{
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		assert(q->head != q->tail || q->lastop != QUE_WRITE);
		return 0;
	}

	q->buf[q->tail] = a;
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

int que_pushe_d(queue_d* q, double a)
{
	double* tmp;
	size_t tmp_sz, inc;
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		tmp_sz = QUE_D_ALLOCATOR(q->capacity);
		inc = tmp_sz - q->capacity;
		if (!(tmp = (double*)realloc(q->buf, sizeof(double)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

		/* hmmm */
		if (q->head) {
			memmove(&q->buf[q->head+inc], &q->buf[q->head], (q->capacity-q->head)*sizeof(double));
		} else {
			q->tail = q->capacity;
		}

		q->capacity = tmp_sz;
	}

	q->buf[q->tail] = a;
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

/*
 * it's programmers responsibility 
 * to make sure it's not empty
*/
double que_pop_d(queue_d* q)
{
	double a;

	assert(q->head != q->tail || q->lastop == QUE_WRITE);

	a = q->buf[q->head];
	q->head++;
	q->head %= q->capacity;
	q->lastop = QUE_READ;

	if (q->head == q->tail)
		q->head = q->tail = 0;

	return a;
}

int que_resize_d(queue_d* q, size_t size)
{
	size_t sz;
	double* tmp = NULL;

	sz = que_size_d(q);
	if (size < sz) {
		assert(size >= sz);
		return 0;
	}

	if (size > q->capacity && q->tail > q->head) {
		if (!(tmp = (double*)realloc(q->buf, sizeof(double)*size))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

	} else {
		tmp = (double*)malloc(size*sizeof(double));
		if (q->tail <= q->head) {
			memcpy(tmp, &q->buf[q->head], (q->capacity-q->head)*sizeof(double));
			memcpy(&tmp[q->capacity-q->head], q->buf, q->tail*sizeof(double));
		} else {
			memcpy(tmp, &q->buf[q->head], (q->tail-q->head)*sizeof(double));
		}

		free(q->buf);
		q->buf = tmp;
		q->head = 0;
		q->tail = sz;
	}

	q->capacity = size;
	return 1;
}

double que_front_d(queue_d* q)
{
	return q->buf[q->head];
}

double que_back_d(queue_d* q)
{
	return q->buf[q->tail-1];
}


int que_is_empty_d(queue_d* q)
{
	return q->head == q->tail && q->lastop == QUE_READ;
}

int que_is_full_d(queue_d* q)
{
	return q->head == q->tail && q->lastop == QUE_WRITE;
}

size_t que_size_d(queue_d* q)
{
	if (q->tail < q->head) {
		return q->capacity - q->head + q->tail;
	} else if (q->tail > q->head) {
		return q->tail - q->head;
	} else if (q->lastop == QUE_WRITE) {
		return q->capacity;
	} else {
		return 0;
	}
}

void free_que_d(void* q)
{
	queue_d* tmp = (queue_d*)q;
	free(tmp->buf);
	tmp->capacity = 0;
}


size_t QUE_STR_START_CAP = 50;

#define QUE_STR_ALLOCATOR(x) ((x) * 2)

char* mystrdup(const char* str)
{
	
	size_t len = strlen(str);
	char* temp = (char*)calloc(len+1, sizeof(char));
	if (!temp) {
		assert(temp != NULL);
		return NULL;
	}
	
	return (char*)memcpy(temp, str, len);  
}

int que_str(queue_str* q, size_t capacity)
{
	q->head = q->tail = 0;
	q->lastop = QUE_READ;
	q->capacity = capacity ? capacity : QUE_STR_START_CAP;

	if (!(q->buf = (char**)malloc(q->capacity*sizeof(char*)))) {
		assert(q->buf != NULL);
		q->capacity = 0;
		return 0;
	}

	return 1;
}

int que_push_str(queue_str* q, char* a)
{
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		assert(q->head != q->tail || q->lastop != QUE_WRITE);
		return 0;
	}

	q->buf[q->tail] = mystrdup(a);
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

int que_pushe_str(queue_str* q, char* a)
{
	char** tmp;
	size_t tmp_sz, inc;
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		tmp_sz = QUE_STR_ALLOCATOR(q->capacity);
		inc = tmp_sz - q->capacity;
		if (!(tmp = (char**)realloc(q->buf, sizeof(char*)*tmp_sz))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

		/* hmmm */
		if (q->head) {
			memmove(&q->buf[q->head+inc], &q->buf[q->head], (q->capacity-q->head)*sizeof(char*));
		} else {
			q->tail = q->capacity;
		}

		q->capacity = tmp_sz;
	}

	q->buf[q->tail] = mystrdup(a);
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

/*
 * it's programmers responsibility 
 * to make sure it's not empty
*/
void que_pop_str(queue_str* q)
{
	assert(q->head != q->tail || q->lastop == QUE_WRITE);

	free(q->buf[q->head]);

	q->head++;
	q->head %= q->capacity;
	q->lastop = QUE_READ;

	if (q->head == q->tail)
		q->head = q->tail = 0;
}

int que_resize_str(queue_str* q, size_t size)
{
	size_t sz;
	char** tmp = NULL;

	sz = que_size_str(q);
	if (size < sz) {
		assert(size >= sz);
		return 0;
	}

	if (size > q->capacity && q->tail > q->head) {
		if (!(tmp = (char**)realloc(q->buf, sizeof(char*)*size))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

	} else {
		tmp = (char**)malloc(size*sizeof(char*));
		if (q->tail <= q->head) {
			memcpy(tmp, &q->buf[q->head], (q->capacity-q->head)*sizeof(char*));
			memcpy(&tmp[q->capacity-q->head], q->buf, q->tail*sizeof(char*));
		} else {
			memcpy(tmp, &q->buf[q->head], (q->tail-q->head)*sizeof(char*));
		}

		free(q->buf);
		q->buf = tmp;
		q->head = 0;
		q->tail = sz;
	}

	q->capacity = size;
	return 1;
}

char* que_front_str(queue_str* q)
{
	return q->buf[q->head];
}

char* que_back_str(queue_str* q)
{
	return q->buf[q->tail-1];
}


int que_is_empty_str(queue_str* q)
{
	return q->head == q->tail && q->lastop == QUE_READ;
}

int que_is_full_str(queue_str* q)
{
	return q->head == q->tail && q->lastop == QUE_WRITE;
}

size_t que_size_str(queue_str* q)
{
	if (q->tail < q->head) {
		return q->capacity - q->head + q->tail;
	} else if (q->tail > q->head) {
		return q->tail - q->head;
	} else if (q->lastop == QUE_WRITE) {
		return q->capacity;
	} else {
		return 0;
	}
}

void free_que_str(void* q)
{
	size_t sz, i;
	queue_str* tmp = (queue_str*)q;
	sz = que_size_str(tmp);
	for (i=0; i<sz; ++i) {
		free(tmp->buf[(tmp->head+i)%tmp->capacity]);
	}
	free(tmp->buf);
	tmp->capacity = 0;
}


size_t QUE_VOID_START_CAP = 50;

#define QUE_VOID_ALLOCATOR(x) ((x) * 2)


int que_void(queue_void* q, size_t capacity, size_t elem_sz, void(*elem_free)(void*), void(*elem_init)(void*, void*))
{
	q->head = q->tail = 0;
	q->lastop = QUE_READ;
	q->capacity = capacity ? capacity : QUE_VOID_START_CAP;

	if (!(q->buf = (byte*)malloc(q->capacity*elem_sz))) {
		assert(q->buf != NULL);
		q->capacity = 0;
		return 0;
	}

	q->elem_size = elem_sz;
	q->elem_free = elem_free;
	q->elem_init = elem_init;

	return 1;
}

int que_push_void(queue_void* q, void* a)
{
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		assert(q->head != q->tail || q->lastop != QUE_WRITE);
		return 0;
	}

	if (q->elem_init) {
		q->elem_init(&q->buf[q->tail*q->elem_size], a);
	} else {
		memcpy(&q->buf[q->tail*q->elem_size], a, q->elem_size);
	}
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

int que_pushe_void(queue_void* q, void* a)
{
	byte* tmp;
	size_t tmp_sz, inc;
	if (q->head == q->tail && q->lastop == QUE_WRITE) {
		tmp_sz = QUE_VOID_ALLOCATOR(q->capacity);
		inc = tmp_sz - q->capacity;
		if (!(tmp = (byte*)realloc(q->buf, tmp_sz*q->elem_size))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

		/* hmmm */
		if (q->head) {
			memmove(&q->buf[(q->head+inc)*q->elem_size], &q->buf[q->head*q->elem_size], (q->capacity-q->head)*q->elem_size);
		} else {
			q->tail = q->capacity;
		}

		q->capacity = tmp_sz;
	}

	if (q->elem_init) {
		q->elem_init(&q->buf[q->tail*q->elem_size], a);
	} else {
		memcpy(&q->buf[q->tail*q->elem_size], a, q->elem_size);
	}
	q->tail++;
	q->tail %= q->capacity;
	q->lastop = QUE_WRITE;
	return 1;
}

/*
 * it's programmers responsibility 
 * to make sure it's not empty
*/
void que_pop_void(queue_void* q)
{
	assert(q->head != q->tail || q->lastop == QUE_WRITE);

	if (q->elem_free) {
		q->elem_free(&q->buf[q->head*q->elem_size]);
	}

	q->head++;
	q->head %= q->capacity;
	q->lastop = QUE_READ;

	if (q->head == q->tail)
		q->head = q->tail = 0;
}

int que_resize_void(queue_void* q, size_t size)
{
	size_t sz;
	byte* tmp = NULL;

	sz = que_size_void(q);
	if (size < sz) {
		assert(size < sz);
		return 0;
	}

	if (size > q->capacity && q->tail > q->head) {
		if (!(tmp = (byte*)realloc(q->buf, size*q->elem_size))) {
			assert(tmp != NULL);
			return 0;
		}
		q->buf = tmp;

	} else {
		tmp = (byte*) malloc(size * q->elem_size);
		if (q->tail <= q->head) {
			memcpy(tmp, &q->buf[q->head*q->elem_size], (q->capacity-q->head)*q->elem_size);
			memcpy(&tmp[(q->capacity-q->head)*q->elem_size], q->buf, q->tail*q->elem_size);

		} else {
			memcpy(tmp, &q->buf[q->head*q->elem_size], (q->tail-q->head)*q->elem_size);
		}

		free(q->buf);
		q->buf = tmp;
		q->head = 0;
		q->tail = sz;
	}

	q->capacity = size;
	return 1;
}

void* que_front_void(queue_void* q)
{
	return (void*)&q->buf[q->head*q->elem_size];
}

void* que_back_void(queue_void* q)
{
	return (void*)&q->buf[(q->tail-1)*q->elem_size];
}


int que_is_empty_void(queue_void* q)
{
	return q->head == q->tail && q->lastop == QUE_READ;
}

int que_is_full_void(queue_void* q)
{
	return q->head == q->tail && q->lastop == QUE_WRITE;
}

size_t que_size_void(queue_void* q)
{
	if (q->tail < q->head) {
		return q->capacity - q->head + q->tail;
	} else if (q->tail > q->head) {
		return q->tail - q->head;
	} else if (q->lastop == QUE_WRITE) {
		return q->capacity;
	} else {
		return 0;
	}
}

void free_que_void(void* q)
{
	size_t sz, i;
	queue_void* tmp = (queue_void*)q;
	sz = que_size_void(tmp);
	if (tmp->elem_free) {
		for (i=0; i<sz; ++i) {
			tmp->elem_free(&tmp->buf[((tmp->head+i)%tmp->capacity) * tmp->elem_size]);
		}
	}
	free(tmp->buf);
	tmp->capacity = 0;
}



/* end CQUEUE_IMPLEMENTATION */
#endif
