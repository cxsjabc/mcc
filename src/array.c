#include <assert.h>
#include <stdio.h>

#include "mcc/array.h"
#include "mcc/log.h"

// basic array funtions
int is_array_equal(int arr[], int expect_arr[], int size)
{
    int i = 0;

    for (i = 0; i < size; i++) {
        if (arr[i] != expect_arr[i])
            return 0;
    }

    return 1;
}

// Dynamic Array
DynArray create_dynamic_array(unsigned int size)
{
    DynArray arr;

    arr = (DynArray)malloc(sizeof(struct dynamic_array));
    if (!arr)
        return NULL;

    arr->data = (void **)malloc(sizeof(void *) * size);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    arr->size = size;
    arr->capacity = size;

    arr->compare = NULL;
    arr->to_string = NULL;
    arr->destroy = NULL;

    return arr;
}

void destroy_dynamic_array(DynArray arr)
{
    int i;
    assert(arr);

    for (i = 0; i < arr->size; ++i) {
        if (arr->destroy) {
            arr->destroy(arr->data[i]);
        }
    }

    free(arr);
}

int dynamic_array_push(DynArray arr, void *data)
{
    if (arr->size == arr->capacity) {
        void **new_data = (void **)realloc(arr->data, sizeof(void *) * (arr->capacity * 2));
        if (!new_data)
            return 0;

        arr->data = new_data;
        arr->capacity *= 2;
    }

    arr->data[arr->size++] = data;
    return 1;
}

void *dynamic_array_pop(DynArray arr)
{
    if (arr->size < 1)
        return NULL;
    return arr->data[--arr->size];
}

inline int dynamic_array_is_empty(DynArray arr)
{
    return arr->size == 0;
}

inline int dynamic_array_size(DynArray arr)
{
    return arr->size;
}

void *dynamic_array_get(DynArray arr, int index)
{
    if (index < 0 || index >= arr->size)
        return NULL;
    return arr->data[index];
}

void *dynamic_array_set(DynArray arr, int index, void *data)
{
    if (index < 0 || index >= arr->size)
        return NULL;

    if (arr->destroy)
        arr->destroy(arr->data[index]);
    arr->data[index] = data;
    return data;
}

int is_dynamic_array_equal(DynArray arr, DynArray expect_arr)
{
    int i = 0;

    for (i = 0; i < arr->size; i++) {
        if (arr->compare) {
            if (!arr->compare(arr->data[i], expect_arr->data[i]))
                return 0;
        } else {
            if (arr->data[i] != expect_arr->data[i])
                return 0;
        }
    }
    return 1;
}

void dump_dynamic_array(DynArray arr)
{
    int i;

    if (!arr) {
        error("Dynamic Array(NULL)\n");
        return;
    }

    debug_nofl("Dynamic Array(%p) size(%d) capacity(%d)\n", arr, arr->size, arr->capacity);

    for (i = 0; i < arr->size; ++i) {
        debug_nofl("[%d][%p]", i, arr->data[i]);
        if (arr->to_string)
            debug_nofl(": (%s)", arr->to_string(arr->data[i]));
        debug_nofl("\n");
    }
    debug_nofl("\n");
}