#include "c_utils.h"

#include "rsw_cstr.h"



#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>

const char* test_strs[] =
{ "Hello World!",
  "The quick brown fox jumped over the lazy dog.",
  "    		blah1 blah2 blah3\n    \v  "
};

c_array d;


void mytoupper(const void* c)
{
	*(char*)c = toupper(*(char*)c);
}


int main()
{
	int i,j,k;
	c_array a, b = {NULL, 1, 0} , c;
	c_array e = { NULL, 0, 0 };
	c_array slice1, slice2, slice3;
	char** lines;
	c_array lines_array, line_file;
	char* one_line = NULL;
	FILE* file;

	printf("%p, %lu %lu\n", b.data, b.elem_size, b.len);
	printf("%p, %lu %lu\n", d.data, d.elem_size, d.len);
	printf("%p, %lu %lu\n", e.data, e.elem_size, e.len);

	a = init_c_array((byte*)test_strs[0], 1, strlen(test_strs[0]));

	slice1 = slice_c_array(a, 0, a.len);
	slice2 = slice_c_array(a, 1, -1);
	printf("actual string: '%s'\n[0,len]: '%s'\nstr[1,-1]: '%s'\n", a.data, slice1.data, slice2.data);

	char* trim_test = mystrdup(test_strs[2]);

	printf("\"%s\"\n", trim_test);
	printf("\"%s\"\n", ltrim(trim_test));
	printf("\"%s\"\n", rtrim(trim_test));
	free(trim_test);
	trim_test = mystrdup(test_strs[2]);
	printf("\"%s\"\n\n\n", trim(trim_test));
	free(trim_test);


	c_array test_map = init_c_array((byte*)test_strs[1], 1, strlen(test_strs[1]));
	printf("\"%s\"\n", (char*)test_map.data);
	map(&test_map, mytoupper);
	printf("\"%s\"\n", (char*)test_map.data);
	free(test_map.data);


	c_array file_contents, test_split, *results;
	if (!file_open_read("../split_test", "r", &file_contents)) {
		perror("Error opening ../split_test");
		return 0;
	}
	if (!split(&file_contents, (byte*)"!@#", 3, &test_split)) {
		printf("error splitting\n");
	} else {
		rsw_cstr test_concat;
		init_cstr(&test_concat);
		
		printf("spliting results:\n================\n");
		results = (c_array*)test_split.data;
		for (i=0; i<test_split.len; i++) {
			cstr_concatenate(&test_concat, (char*)results[i].data, results[i].len);
			printf("\"");
			for (j=0; j<results[i].len; j++)
				printf("%c", results[i].data[j]);
			printf("\"\n");

			if (i+1 < test_split.len) {
				cstr_concatenate(&test_concat, "!@#", 3);
				//cstr_push(&test_concat, '!');
				//cstr_push(&test_concat, '@');
				//cstr_push(&test_concat, '#');
			}
		}
		printf("\n================\n");
		free(test_split.data);
		free(file_contents.data);
		
		printf("\n\"%s\"\n\n", test_concat.a);
		file_open_write_cstr("../concat_results", &test_concat);
		free_cstr(&test_concat);
	}



/*
	file_open_readlines("../test_readlines", &lines_array, &line_file);
	for (i=0; i<lines_array.len; i++) {
 	   printf("\"%s\"\n", ((char**)lines_array.data)[i]);
   	}

	printf("\n\n\n\n");
	file = fopen("../test_readlines", "r");
	if (file) {
		while(one_line = freadline(file)) {
			printf("\"%s\"\n", one_line);
			free(one_line);
		}
	}
	fclose(file);
	free(lines_array.data);
	free(line_file.data);

*/


	free(slice1.data);
	free(slice2.data);
	free(a.data);

#define HAYSTACK_LEN 10000
#define NEEDLE_LEN 10
	c_array search_test, search_str;

	byte* haystack =(byte*) malloc(HAYSTACK_LEN);
	byte* needle = (byte*) malloc(NEEDLE_LEN+1);
	search_test = init_c_array(haystack, 1, HAYSTACK_LEN);  assert(search_test.data);
	search_str = init_c_array(needle, 1, NEEDLE_LEN);  assert(search_str.data);
	free(haystack);
	free(needle);

	srand(time(NULL));
	for (i=0; i<HAYSTACK_LEN; ++i) {
		search_test.data[i] = rand() % 256;
		//printf("%d ", search_test.data[i]);
	}
	printf("\n");

	for (i=0; i<NEEDLE_LEN; ++i)
		search_str.data[i] = i;


	int pos;
	for (i=0; i<3; ++i) {
		pos = rand() % (HAYSTACK_LEN-NEEDLE_LEN);
		printf("pos %d = %d\n", i, pos);
		memcpy(&search_test.data[pos], search_str.data, NEEDLE_LEN);
	}

	pos = 6253;//rand() % (HAYSTACK_LEN-NEEDLE_LEN);
	printf("pos %d = %d\n", i, pos);
	int some_num = 100;
	memcpy(&search_test.data[pos], &some_num, sizeof(int));

/*
	for (i=0; i<HAYSTACK_LEN; ++i) {
		printf("%d ", search_test.data[i]);
	}
	printf("\n");
*/
	c_array search_int = init_c_array((byte*)&some_num, 1, sizeof(int));
	//boyermoore_search(search_test, search_str);
	//boyermoore_search(search_test, search_int);

	printf("basic search:\n");
//	find(search_test, search_str);
	find(search_test, search_int);

	free(search_int.data);
	free(search_test.data);
	free(search_str.data);


	double test_double_array[100];
	int test_int_array[100];

	for (int i=0; i<100; ++i) {
		test_double_array[i] = rand() / (rand()*1.0f);
		test_int_array[i] = rand();
	}

	int to_find_int = test_int_array[rand()%100];
	double to_find_double = test_double_array[rand()%100];
	c_array int_c_array;
	c_array double_c_array;


	SET_C_ARRAY(int_c_array, (byte*)test_int_array, sizeof(int), 100);
	SET_C_ARRAY(double_c_array, (byte*)test_double_array, sizeof(double), 100);

	printf("int is_any = %d\n", is_any(&int_c_array, (byte*)&to_find_int, are_equal_int));
	printf("double is_any = %d\n", is_any(&double_c_array, (byte*)&to_find_double, are_equal_double));


	if (!file_open_read("../split_test", "r", &file_contents)) {
		perror("Error opening ../split_test");
		return 0;
	}

	rsw_cstr file_str, delim;
	
	init_cstr_str(&file_str, (char*)file_contents.data, file_contents.len);
	free(file_contents.data);
	init_cstr_str(&delim, "!@#", 3);

	rsw_cstr* cstr_results;
	size_t num_results;
	cstr_split(&file_str, &delim, &cstr_results, &num_results);

	for (int i=0; i<num_results; ++i)
		printf("%d = \"%s\"\n", i, cstr_results[i].a);

	for (int i=0; i<num_results; ++i)
		free_cstr(&cstr_results[i]);

	free(cstr_results);



	if (!file_open_read_cstr("../alt-2600-hack-faq.txt", &file_str)) {
		perror("Error opening ../alt-2600-hack-faq.txt");
		return 0;
	}
	//cstr_set_str(&file_str, (char*)file_contents.data, file_contents.len);
	
	cstr_set_str(&delim, "password", 8);
	
	cstr_split(&file_str, &delim, &cstr_results, &num_results);
	printf("num_results = %u\n", num_results);

	/*
	for (int i=0; i<num_results; ++i)
		printf("%d = \"%s\"\n", i, cstr_results[i].a);
		*/

	for (int i=0; i<num_results; ++i)
		free_cstr(&cstr_results[i]);

	free(cstr_results);
	free_cstr(&delim);


	if (!file_open_read_cstr("../replace_test", &file_str)) {
		perror("Error opening ../replace_test");
		return 0;
	}

	size_t not_found = -1;
	size_t start_at = 0;
	size_t result;
	rsw_cstr hello_str = { "hello", 5, 6 };
	while ((result = cstr_find_start_at(&file_str, &hello_str, start_at)) != (size_t)-1) {
		cstr_replace(&file_str, result, 5, "goodbye", 7);
		start_at += 3;
	}

	file_open_write_cstr("../replace_results", &file_str);
	printf("\"%s\"\n", file_str.a);


	free_cstr(&file_str);

	return 0;
}

