#define NO_DEBUG 1

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/array.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"

__BEGIN_DECLS

void test_int_array()
{
	DynArray arr;
	char *s;
	int i, r;
	MemBuf buf;

	buf = init_mem_buf(0);
	assert(buf);

	arr = dynamic_array_create(2);
	assert(arr);

	//arr->compare = strcmp;
	//arr->destroy = mcc_free;
	arr->to_string = (to_string)(void *)0x1;

	for (i = 0; i < 20; ++i) {
		// s = mcc_malloc(sizeof(char) * 16);
		s = (char *)alloc_from_mem_buf(buf, 16);
		assert(s);
		sprintf(s, "%d", i);

		r = dynamic_array_push(arr, (void *)(intptr_t)i);
		assert(r > 0);
	}

	dump_dynamic_array(arr);

	dynamic_array_destroy(arr);
}

char *char_str_to_string(void *data)
{
	char *p = (char *)data;
	return p;
}

void test_str_array()
{
	DynArray arr;
	char *s;
	int i, r;
	MemBuf buf;

	buf = init_mem_buf(0);
	assert(buf);

	arr = dynamic_array_create(2);
	assert(arr);

	//arr->compare = strcmp;
	arr->destroy = mcc_free;
	arr->to_string = char_str_to_string;

	for (i = 0; i < 20; ++i) {
		// s = mcc_malloc(sizeof(char) * 16);
		s = (char *)alloc_from_mem_buf(buf, 16);
		assert(s);
		sprintf(s, "string %d", i);

		r = dynamic_array_push(arr, s);
		assert(r > 0);
	}

	dump_dynamic_array(arr);

	dynamic_array_destroy(arr);
	// dump_dynamic_array(arr);
}

void test_array()
{
	test_str_array();

	test_int_array();

	return;
}

__END_DECLS