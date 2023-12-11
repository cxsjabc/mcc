#ifndef MEM_H
#define MEM_H

#include "mcc/features.h"

#ifndef __cplusplus
#define mcc_malloc malloc
#define mcc_free free

#define mcc_calloc calloc
#define mcc_realloc realloc
#else
#define mcc_malloc cpp_malloc
#define mcc_free cpp_free

#define mcc_calloc cpp_calloc
#define mcc_realloc cpp_realloc

void *cpp_malloc(size_t size);
void cpp_free(void *p);
void *cpp_calloc(size_t num, size_t size);
void *cpp_realloc(void *p, size_t size);
#endif

// All threads will share the same memory buffer.
// It's important to make them synchronous.
#define allocm(size) alloc_from_mem_buf(&GlobalMemBuf, size)

#endif