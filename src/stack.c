#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "mcc/stack.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/print.h"

__BEGIN_DECLS

Stack stack_create(unsigned int ele_size)
{
	Stack st;

	st = (Stack)mcc_malloc(sizeof(struct stack));
	if (!st)
		return NULL;

	if (ele_size != 0) {
		st->data = (void **)mcc_malloc(sizeof(void *) * ele_size);
		if (!st->data) {
			mcc_free(st);
			return NULL;
		}
	} else
		st->data = NULL;

	st->size = 0;
	st->capacity = ele_size;

	st->compare = NULL;
	st->to_string = NULL;  // means default "char *"
	st->destroy = NULL;

	return st;
}

void stack_destroy(Stack st)
{
	int i;
	assert(st);

	st->data = NULL;
	st->size = 0;

	for (i = 0; i < st->size; ++i) {
		if (st->destroy) {
			st->destroy(st->data[i]);
		}
	}

	mcc_free(st);
}

int stack_push(Stack st, void *data)
{
	if (st->size == st->capacity) {
		size_t orig_size;
		size_t new_size;
		void **new_data;

		LHD;
		orig_size = sizeof(void *) * st->capacity;
		if (orig_size == 0)
			orig_size = 1;
		new_size  = orig_size << 1;
		new_data = (void **)mcc_realloc_safe(st->data, orig_size, new_size);
		if (!new_data)
			fatal("No enough memory, crash!\n");

		st->data = new_data;
		st->capacity *= 2;
	}

	st->data[st->size++] = data;
	return 1;
}

void *stack_pop(Stack st)
{
	if (st->size < 1)
		return NULL;
	return st->data[--st->size];
}

void *stack_top(Stack st)
{
	if (st->size < 1)
		return NULL;
	return st->data[st->size - 1];
}

inline int stack_is_empty(Stack st)
{
	return st->size == 0;
}

inline int stack_size(Stack st)
{
	return st->size;
}

void *stack_get(Stack st, int index)
{
	if (index < 0 || index >= st->size)
		return NULL;
	return st->data[index];
}

void *stack_set(Stack st, int index, void *data)
{
	if (index < 0 || index >= st->size)
		return NULL;

	if (st->destroy)
		st->destroy(st->data[index]);
	st->data[index] = data;
	return data;
}

int is_stack_equal(Stack st, Stack expect_st)
{
	int i = 0;

	for (i = 0; i < st->size; i++) {
		if (st->compare) {
			if (!st->compare(st->data[i], expect_st->data[i]))
				return 0;
		} else {
			if (st->data[i] != expect_st->data[i])
				return 0;
		}
	}
	return 1;
}

void dump_stack(Stack st)
{
	int i;

	if (!st) {
		error("Stack(NULL)\n");
		return;
	}

	debug_nofl("Stack(%p) size(%d) capacity(%d)\n", st, st->size, st->capacity);

	for (i = 0; i < st->size; ++i) {
		debug_nofl("[%d][%p]", i, st->data[i]);
		if (st->to_string == NULL)
			debug_nofl(": (%s)", (char *)(st->data[i]));
		else if((intptr_t)st->to_string == 0x1) {
			debug_nofl(": ("); print_pointer(st->data[i]); debug_nofl(")"); //debug_nofl(": (%#llx)", (intptr_t)(st->data[i]));
		} else if (st->to_string)
			debug_nofl(": (%s)", st->to_string(st->data[i]));
		
		debug_nofl("\n");
	}
	debug_nofl("\n");
}

__END_DECLS