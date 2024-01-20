#ifndef MEM_H
#define MEM_H

#include <string.h>

#include "mcc/mcc_base.h"
#include "mcc/mcc_state.h"
#include "mcc/mem_buf.h"

#define USE_MEM_BUF 1

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
#if USE_MEM_BUF
void *allocm(size_t size);
#define freem(p)
#else
#define allocm(size) mcc_malloc(size)
#define freem(p) mcc_free(p)
#endif

void *allocmz(size_t size);
void *mcc_realloc_safe(void *p, size_t orig_size, size_t new_size);

#endif