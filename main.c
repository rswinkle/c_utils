#include "c_utils.h"

#include "rsw_cstr.h"



#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <CUnit/Automated.h>


void file_read_test()
{
	c_array file_contents;
	int size = file_open_read("../split_test", "r", &file_contents);

	CU_ASSERT_EQUAL(size, 164);

	CU_ASSERT_EQUAL(file_contents.elem_size, 1);
	CU_ASSERT_EQUAL(file_contents.len, size);


	free(file_contents.data);
}

void file_read_cstr_test()
{
	rsw_cstr file_str;
	int size = file_open_read_new_cstr("../alt-2600-hack-faq.txt", &file_str);

	CU_ASSERT_EQUAL(size, 247708);

	CU_ASSERT_EQUAL(file_str.size, size);
	CU_ASSERT_EQUAL(file_str.capacity, size+1); //still haven't decided if I'll change this capacity

	free_cstr(&file_str);
}



CU_TestInfo c_utils_tests[] = {
	{ "file_read",           file_read_test },
	CU_TEST_INFO_NULL
};

CU_TestInfo rsw_cstr_tests[] = {
	{ "file_read_cstr",      file_read_cstr_test },
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

