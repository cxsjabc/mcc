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
void stack_destroy(Stack arr);

int stack_push(Stack arr, void *data);
void *stack_pop(Stack arr);

int stack_is_empty(Stack arr);
int stack_size(Stack arr);

void *stack_get(Stack arr, int index);
void *stack_set(Stack arr, int index, void *data);
int is_stack_equal(Stack arr, Stack expect_arr);

void dump_stack(Stack arr);

__END_DECLS

#endif