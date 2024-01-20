#ifndef STACK_H
#define STACK_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

typedef char *(*to_string)(void*);

typedef struct stack
{
	void **data;
	int size;  // element count
	int capacity;

	char *(*to_string)(void *); // NULL: char *, 0x1: basic type, others: normal pointer - use to_string specific func. 
	int (*compare)(void *, void *);
	void (*destroy)(void *);
} *Stack;

Stack stack_create(unsigned int size);
void stack_destroy(Stack st);

int stack_push(Stack st, void *data);
void *stack_pop(Stack st);
void *stack_top(Stack st);

int stack_is_empty(Stack st);
int stack_size(Stack st);

void *stack_get(Stack st, int index);
void *stack_set(Stack st, int index, void *data);
int is_stack_equal(Stack st, Stack expect_arr);

void dump_stack(Stack st);

__END_DECLS

#endif