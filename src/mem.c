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

void *mcc_realloc_safe(void *p, size_t orig_size, size_t *new_size)
{
	void *p1;
	size_t diff;

	diff = *new_size - orig_size;
	assert(*new_size >= orig_size);
	while ((p1 = mcc_realloc(p, *new_size)) == NULL) {
		*new_size -= (diff >> 1);
		diff = *new_size - orig_size;
		if (diff <= 8)
			fatal("Seems no any memory, crash!\n");
	}
	return p1;
}

void *mcc_mallocz(size_t size)
{
	void *p = mcc_malloc(size);

	if (p)
		memset(p, 0, size);
	return p;
}

void mem_print(void *p, int unit_len, const char *prefix)
{
	unsigned char *pc;
	unsigned short *ps;
	unsigned int *pi;
	unsigned long long *pl;

	if (unit_len != 1 && unit_len % 2 != 0) {
		error("unit_len(%d) must be 1 or multiple of 2\n", unit_len);
		return;
	}

	if (unit_len <= 0 || unit_len > 8) {
		error("should be: 1 <= unit_len(%d) <= 8\n", unit_len);
		return;
	}

	if (unit_len == 1) {
		pc = (unsigned char *)p;
		always("%s", prefix), always("0x%02x", *pc);
	}
	else if (unit_len == 4) {
		pi = (unsigned int *)p;
		always("%s", prefix), always("0x%08x", *pi);
	}
	else if (unit_len == 8) {
		pl = (unsigned long long *)p;
		always("%s", prefix), always("0x%016llx", *pl);
	}
	else {
		ps = (unsigned short *)p;
		always("%s", prefix), always("0x%04x", *ps);
	}
}

void mem_dump(void *p, int size, int unit, int line_element_size)
{
	char *pc = (char *)p;
	int i;

	if (unit != 1 && unit % 2 != 0) {
		error("unit(%d) must be 1 or multiple of 2\n", unit);
		return;
	}

	if (unit <= 0 || unit > 8) {
		error("should be: 1 <= unit(%d) <= 8\n", unit);
		return;
	}

	if (line_element_size < 1)
		line_element_size = 16;

	for (i = 0; i * unit < size; i++) {
		if (i % line_element_size == 0)
			mem_print(pc + i * unit, unit, "");
		else
			mem_print(pc + i * unit, unit, " ");
		if ((i + 1) % line_element_size == 0)
			always("\n");
	}
	always("\n");
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