#include "c_utils.h"

#include "rsw_cstr.h"


#define PRIORITY_QUEUE_IMPLEMENTATION
#include "priority_queue.h"



#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <CUnit/Automated.h>


void file_rw_test()
{
	c_array file_contents, file_contents2;
	int size = file_open_read("../alt-2600-hack-faq.txt", "r", &file_contents);

	CU_ASSERT_EQUAL(size, 247708);

	CU_ASSERT_EQUAL(file_contents.elem_size, 1);
	CU_ASSERT_EQUAL(file_contents.len, size);

	file_open_write("../alt-2600.out", "w", &file_contents);

	FILE* file = fopen("../alt-2600.out", "r");
	size = file_read(file, &file_contents2);
	CU_ASSERT_EQUAL(size, 247708);

	CU_ASSERT_EQUAL(file_contents.elem_size, file_contents2.elem_size);
	CU_ASSERT_EQUAL(file_contents.len, file_contents2.len);

	CU_ASSERT_STRING_EQUAL(file_contents.data, file_contents2.data);

	free(file_contents.data);
	free(file_contents2.data);
}




void file_rw_cstr_test()
{
	rsw_cstr file_str;
	int size = file_open_read_new_cstr("../alt-2600-hack-faq.txt", &file_str);

	CU_ASSERT_EQUAL(size, 247708);

	CU_ASSERT_EQUAL(file_str.size, size);
	CU_ASSERT_EQUAL(file_str.capacity, size+1); //still haven't decided if I'll change this capacity

	file_open_write_cstr("../alt_2600.out", &file_str);

	FILE* file = fopen("../alt_2600.out", "r");
	rsw_cstr file_str2;
	init_cstr(&file_str2);

	size = file_read_cstr(file, &file_str2);
	CU_ASSERT_EQUAL(size, 247708);

	CU_ASSERT_EQUAL(file_str2.size, size);
	CU_ASSERT_STRING_EQUAL(file_str.a, file_str2.a);

	free_cstr(&file_str);
	free_cstr(&file_str2);
}

void bottles_of_beer_test()
{
	const char verse[] = "#a bottles of beer on the wall, #a bottles of beer, take one down, pass it around, #b bottles of beer on the wall.";
	const char verse0[] = "No more bottles of beer on the wall, no more bottles of beer.\nGo to the store, buy some more, 99 bottles of beer on the wall.";
	const char verse1[] = "1 bottle of beer on the wall, 1 bottle of beer.\nTake one down, pass it around, no more bottles of beer on the wall.";
	const char of_beer[] = "of beer";
	const char wall[] = "on the wall";
	const char take_one[] = "Take one down, pass it around,";

	int len = strlen(verse);

	char int_buf[5], int_buf2[5];

	rsw_cstr template_str;
//	init_cstr_str(&template_str, verse, len);

	int tmp;

#define NUM_BOTTLES 5

	FILE* file1 = fopen("bottles1.txt", "w");
	for (int i=NUM_BOTTLES; i>=0; --i) {
		switch (i) {
		case 0:
			fprintf(file1, "No more bottles %s %s, no more bottles %s.\nGo to the store, buy some more, 99 bottles %s %s.\n", of_beer, wall, of_beer, of_beer, wall);
			break;
		case 1:
			fprintf(file1, "1 bottle %s %s, 1 bottle %s.\n%s no more bottles %s %s.\n\n", of_beer, wall, of_beer, take_one, of_beer, wall);
			break;
		default:
			fprintf(file1, "%d bottles %s %s, %d bottles %s.\n", i, of_beer, wall, i, of_beer);
			fprintf(file1, "%s %d %s %s %s.\n\n", take_one, i-1, (i!=2) ? "bottles" : "bottle", of_beer, wall);  
		}
	}
	fclose(file1);

	rsw_cstr bottle_str;
	int size = file_open_read_new_cstr("bottles1.txt", &bottle_str);
	

	rsw_cstr song;
	init_cstr(&song);
	for (int i=NUM_BOTTLES; i>=0; --i) {
		switch (i) {
		case 0:
			cstr_concatenate(&song, verse0, strlen(verse0));
			cstr_push(&song, '\n');
			break;
		case 1:
			cstr_concatenate(&song, verse1, strlen(verse1));
			cstr_push(&song, '\n');
			cstr_push(&song, '\n');
			break;
		default:
			snprintf(int_buf, 5, "%d", i);
			snprintf(int_buf2, 5, "%d", i-1);
			cstr_concatenate(&song, int_buf, strlen(int_buf));
			cstr_concatenate(&song, " bottles ", 9);
			cstr_concatenate(&song, of_beer, strlen(of_beer));
			cstr_push(&song, ' ');
			cstr_concatenate(&song, wall, strlen(wall));
			cstr_concatenate(&song, ", ", 2);
			cstr_concatenate(&song, int_buf, strlen(int_buf));
			cstr_concatenate(&song, " bottles ", 9);
			cstr_concatenate(&song, of_beer, strlen(of_beer));
			cstr_concatenate(&song, ".\n", 2);

			cstr_concatenate(&song, take_one, strlen(take_one));
			cstr_push(&song, ' ');
			cstr_concatenate(&song, int_buf2, strlen(int_buf2));
			if (i != 2)
				cstr_concatenate(&song, " bottles ", 9);
			else 
				cstr_concatenate(&song, " bottle ", 8);

			cstr_concatenate(&song, of_beer, strlen(of_beer));
			cstr_push(&song, ' ');
			cstr_concatenate(&song, wall, strlen(wall));
			cstr_concatenate(&song, ".\n\n", 3);
		}
	}

	CU_ASSERT_STRING_EQUAL(song.a, bottle_str.a);

	cstr_replace(&song, "beer", "soda", 0);
	printf("\"%s\"\n\n", song.a);
	cstr_replace(&song, "wall", "shelf", 0);
	printf("\"%s\"\n\n", song.a);

	free_cstr(&song);
	free_cstr(&bottle_str);
}


void priority_queue_test()
{
	int heap[100];
	size_t sz = 0, cap = 100;
	int a, b = INT_MAX;

	for (int i=0; i<50; ++i)
		maxheap_push(heap, &sz, cap, rand() % 400);

	for (int i=0; i<25; ++i) {
		a = maxheap_pop(heap, &sz);
		CU_ASSERT(a <= b);
		//printf("%d\n", a);
		b = a;
	}
	putchar('\n');

	for (int i=0; i<20; ++i)
		maxheap_push(heap, &sz, cap, rand() % 400);

	b = INT_MAX;
	while (sz > 0) {
		a = maxheap_pop(heap, &sz);
		CU_ASSERT(a <= b);
		//printf("%d\n", a);
		b = a;
	}
}


CU_TestInfo c_utils_tests[] = {
	{ "file_rw",           file_rw_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo rsw_cstr_tests[] = {
	{ "file_rw_cstr",      file_rw_cstr_test },
	{ "bottles_of_beer",   bottles_of_beer_test },
	CU_TEST_INFO_NULL
};


CU_TestInfo priority_queue_tests[] = {
	{ "pqtest",           priority_queue_test },
	CU_TEST_INFO_NULL
};

CU_SuiteInfo suites[] = {
#ifndef OLD_CUNIT
	{ "c_utils", NULL, NULL, NULL, NULL, c_utils_tests },
	{ "rsw_cstr", NULL, NULL, NULL, NULL, rsw_cstr_tests },
	{ "priority_queue_tests", NULL, NULL, NULL, NULL, priority_queue_tests },
#else
	{ "c_utils", NULL, NULL, c_utils_tests },
	{ "rsw_cstr", NULL, NULL, rsw_cstr_tests },
	{ "priority_queue_tests", NULL, NULL, priority_queue_tests },
#endif
	CU_SUITE_INFO_NULL
};



int main()
{
	CU_ErrorCode error;
	error = CU_initialize_registry();
	if (error != CUE_SUCCESS) {
		fprintf(stderr, "Failed to initialize registry\n");
		return CU_get_error();
	}

	error = CU_register_suites(suites);

	if (error != CUE_SUCCESS) {
		fprintf(stderr, "Failed to register test suite\n");
		return CU_get_error();
	}

	CU_automated_run_tests();
	fprintf(stdout, "CU_get_error() returned %d\n", CU_get_error());

	CU_cleanup_registry();

	return CU_get_error();

}

