#include "c_utils.h"

#include "rsw_cstr.h"



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
}

void bottles_of_beer_test()
{
	const char verse[] = "#a bottles of beer on the wall, #a bottles of beer, take one down, pass it around, #b bottles of beer on the wall";
	int len = strlen(verse);

	char int_buf[5], int_buf2[5];

	rsw_cstr template_str;
	init_cstr_str(&template_str, verse, len);

	rsw_cstr song, verse_str;
	init_cstr(&song);
	init_cstr(&verse_str);
	int tmp;
	for (int i=5; i>0; --i) {
		snprintf(int_buf, 5, "%d", i);
		snprintf(int_buf2, 5, "%d", i-1);
		cstr_copy(&verse_str, &template_str);

		cstr_replace(&verse_str, "#a", int_buf, 0);
		cstr_replace(&verse_str, "#b", int_buf2, 0);

		cstr_concatenate_cstr(&song, &verse_str);
	}

}



CU_TestInfo c_utils_tests[] = {
	{ "file_rw",           file_rw_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo rsw_cstr_tests[] = {
	{ "file_rw_cstr",      file_rw_cstr_test },
	CU_TEST_INFO_NULL
};


CU_SuiteInfo suites[] = {
	{ "c_utils", NULL, NULL, c_utils_tests },
	{ "rsw_cstr", NULL, NULL, rsw_cstr_tests },
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

