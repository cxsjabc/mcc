#define NO_DEBUG 0

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/stack.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"

__BEGIN_DECLS

void test_int_stack()
{
	Stack st;
	char *s;
	int i, r;
	MemBuf buf;

	buf = init_mem_buf(0);
	assert(buf);

	st = stack_create(2);
	assert(st);

	//st->compare = strcmp;
	//st->destroy = mcc_free;
	st->to_string = (to_string)(void *)0x1;

	for (i = 0; i < 20; ++i) {
		// s = mcc_malloc(sizeof(char) * 16);
		s = (char *)alloc_from_mem_buf(buf, 16);
		assert(s);
		sprintf(s, "%d", i);

		r = stack_push(st, (void *)(intptr_t)i);
		assert(r > 0);
	}

	dump_stack(st);

	stack_destroy(st);
}

static char *char_str_to_string(void *data)
{
	char *p = (char *)data;
	return p;
}

void test_str_stack()
{
	Stack st;
	char *s;
	int i, r;
	MemBuf buf;

	buf = init_mem_buf(0);
	assert(buf);

	st = stack_create(2);
	assert(st);

	//st->compare = strcmp;
	st->destroy = mcc_free;
	st->to_string = char_str_to_string;

	for (i = 0; i < 20; ++i) {
		// s = mcc_malloc(sizeof(char) * 16);
		s = (char *)alloc_from_mem_buf(buf, 16);
		assert(s);
		sprintf(s, "string %d", i);

		r = stack_push(st, s);
		assert(r > 0);
	}
	dump_stack(st);

	for (i = 19; i >= 0; --i) {
		char *p = stack_pop(st);
		assert(p);

		s = (char *)alloc_from_mem_buf(buf, 16);
		assert(s);
		sprintf(s, "string %d", i);
		assert(STR_EQL(p, s));
		silence("Pop element: %s\n", p);
	}
	dump_stack(st);

	stack_destroy(st);
	// dump_stack(st);
}

void test_stack()
{
	test_str_stack();

	test_int_stack();

	return;
}

__END_DECLS