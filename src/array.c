#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/print.h"

__BEGIN_DECLS

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

int is_array_element_equal(void *arr, int size, int unit)
{
	int i = 0;

	assert(size % unit == 0);
	assert(unit == 4 || unit == 8);

	for (i = 0; i < size / unit - 1; i++) {
		if (((unsigned long *)arr)[i] != ((unsigned long *)arr)[i + 1]) {
			error("Index(%d, %d) is not equal, value(0x%lx, 0x%lx).\n", i, i + 1, ((unsigned long *)arr)[i], ((unsigned long *)arr)[i + 1]);
			return 0;
		}
	}

	return 1;
}

void dump_array(void *arr, int size)
{
	unsigned long *pi = (unsigned long *)arr;
	int i;

	for (i = 0; i < size; i++) {
		if (i % 16 == 0)
			debug_nofl("0x%lx", pi[i]);
		else
			debug_nofl(" 0x%lx", pi[i]);
		if ((i + 1) % 16 == 0)
			debug_nofl("\n");
	}
	debug_nofl("\n");
}

// Dynamic Array
DynArray dynamic_array_create(unsigned int size)
{
	DynArray arr;

	arr = (DynArray)mcc_malloc(sizeof(struct dynamic_array));
	if (!arr)
		return NULL;

	if (size != 0) {
		arr->data = (void **)mcc_malloc(sizeof(void *) * size);
		if (!arr->data) {
			mcc_free(arr);
			return NULL;
		}
	} else
		arr->data = NULL;

	arr->size = 0;
	arr->capacity = size;

	arr->compare = NULL;
	arr->to_string = NULL;  // means default "char *"
	arr->destroy = NULL;

	return arr;
}

void dynamic_array_destroy(DynArray arr)
{
	int i;
	assert(arr);

	arr->data = NULL;
	arr->size = 0;

	for (i = 0; i < arr->size; ++i) {
		if (arr->destroy) {
			arr->destroy(arr->data[i]);
		}
	}

	mcc_free(arr);
}

int dynamic_array_push(DynArray arr, void *data)
{
	if (arr->size == arr->capacity) {
		size_t orig_size = sizeof(void *) * arr->capacity;
		size_t new_size = orig_size << 1;
		void **new_data = (void **)mcc_realloc_safe(arr->data, orig_size, new_size);
		if (!new_data) {
			fatal("No enough memory, crash!\n");
			return ERR_NO_MEM;
		}

		arr->data = new_data;
		arr->capacity *= 2;
	}

	arr->data[arr->size++] = data;
	return OK;
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

void dynamic_array_dump(DynArray arr)
{
	int i;

	if (!arr) {
		error("Dynamic Array(NULL)\n");
		return;
	}

	debug_nofl("Dynamic Array(%p) size(%d) capacity(%d)\n", arr, arr->size, arr->capacity);

	for (i = 0; i < arr->size; ++i) {
		debug_nofl("[%d][%p]", i, arr->data[i]);
		if (arr->to_string == NULL)
			debug_nofl(": (%s)", (char *)(arr->data[i]));
		else if((intptr_t)arr->to_string == 0x1) {
			debug_nofl(": ("); print_pointer(arr->data[i]); debug_nofl(")"); //debug_nofl(": (%#llx)", (intptr_t)(arr->data[i]));
		} else if (arr->to_string)
			debug_nofl(": (%s)", arr->to_string(arr->data[i]));
		
		debug_nofl("\n");
	}
	debug_nofl("\n");
}

__END_DECLS