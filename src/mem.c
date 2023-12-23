#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/mem.h"

__BEGIN_DECLS

void *allocm(size_t size)
{
	MccState ms;
	MemBuf mb;

	ms = (MccState)pthread_getspecific(get_mcc_thread_key());
	if (!ms)
		mb = &GlobalMemBuf;
	else
		mb = ms->global_buf;
	return alloc_from_mem_buf(mb, size);
}

void *allocmz(size_t size)
{
	void *p = allocm(size);
	if (p)
		memset(p, 0, size);
	return p;
}

__END_DECLS

#ifdef __cplusplus
void *cpp_malloc(size_t size)
{
	char *p = new char[size];
	return p;
}

void cpp_free(void *p)
{
	delete[] (char *)p;
}

void *cpp_calloc(size_t num, size_t size)
{
	char *p = new char[num * size];

	if (p)
		memset(p, 0, num * size);

	return p;
}

void *cpp_realloc(void *p, size_t size)
{
	char *q = new char[size];

	if (!q)
		return NULL;

	memcpy(q, p, size);
	delete[] (char *)p;

	return q;
}
#endif