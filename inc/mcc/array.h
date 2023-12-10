#ifndef ARRAY_H
#define ARRAY_H

int is_array_equal(int arr[], int expect_arr[], int size);

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

DynArray create_dynamic_array(unsigned int size);
void destroy_dynamic_array(DynArray arr);

int dynamic_array_push(DynArray arr, void *data);
void *dynamic_array_pop(DynArray arr);

int dynamic_array_is_empty(DynArray arr);
int dynamic_array_size(DynArray arr);

void *dynamic_array_get(DynArray arr, int index);
void *dynamic_array_set(DynArray arr, int index, void *data);
int is_dynamic_array_equal(DynArray arr, DynArray expect_arr);

void dump_dynamic_array(DynArray arr);

#endif