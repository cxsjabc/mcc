#ifndef ARRAY_H
#define ARRAY_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

int is_array_equal(int arr[], int expect_arr[], int size);
int is_array_element_equal(void *arr, int size, int unit);
void dump_array(void *arr, int size);

typedef char *(*to_string)(void*);

typedef struct dynamic_array
{
	void **data;
	int size;
	int capacity;

	char *(*to_string)(void *); // NULL: char *, 0x1: basic type, normal pointer: use to_string specific func. 
	int (*compare)(void *, void *);
	void (*destroy)(void *);
} *DynArray;

DynArray dynamic_array_create(unsigned int size);
void dynamic_array_destroy(DynArray arr);

int dynamic_array_push(DynArray arr, void *data);
void *dynamic_array_pop(DynArray arr);

int dynamic_array_is_empty(DynArray arr);
int dynamic_array_size(DynArray arr);

void *dynamic_array_get(DynArray arr, int index);
void *dynamic_array_set(DynArray arr, int index, void *data);
int is_dynamic_array_equal(DynArray arr, DynArray expect_arr);

void dynamic_array_dump(DynArray arr);
void dynamic_array_dump_prepostfix(DynArray arr, const char *prefix, const char *postfix);

// Here, use lower case macro names for easily coding
#define vec_at(arr, i) dynamic_array_get((arr), (i))
#define vec_size(arr) dynamic_array_size(arr)
#define vec_dump(arr) dynamic_array_dump(arr)
#define vec_dump1(arr, prefix, postfix) dynamic_array_dump_prepostfix((arr), (prefix), (postfix))

__END_DECLS

#endif