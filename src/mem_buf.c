#define NO_DEBUG 1

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"

__BEGIN_DECLS

// global memory buffer
struct mem_buf GlobalMemBuf;

void setup_global_mem_buf()
{
	init_from_mem_buf(&GlobalMemBuf);
}

void uninit_global_mem_buf()
{
	free_mem_buf(&GlobalMemBuf);
}

// init functions
MemBuf init_mem_buf(int size)
{
	MemBuf buf;
	MemChunk chk;

	buf = (MemBuf)mcc_malloc(sizeof(struct mem_buf));
	if (buf == NULL)
		return NULL;

	buf->avail = NULL;
	buf->head.data = NULL;
	buf->head.next = NULL;

#if ENABLE_MEM_INFO
	buf->meminfo.next = buf->meminfo.prev = NULL;
#endif

	if (size > 0) {
		chk = alloc_mem_chunk(size);
		assert(chk);

		buf->head.next = chk;
		buf->avail = chk;

#if ENABLE_MEM_INFO
		ALLOC_MEM_INFO(chk, chk->data, size, 0);
#endif
	}

	return buf;
}

void init_from_mem_buf(MemBuf buf)
{
	memset(buf, 0, sizeof(struct mem_buf));
}

// alloc memory block from mem buffer
void *alloc_from_mem_buf(MemBuf buf, int size)
{
	MemChunk chk;
	void *r;
	const int orig_size = size;

	(void)orig_size;	// make compiler shut up

	chk = buf->avail;
#if ENABLE_MEM_DEBUG
	size += CHK_MEM_BLOCK_MAGIC_SIZE;
#endif

	if (chk == NULL) {
		LHD;
		chk = alloc_mem_chunk(size);
		assert(chk);
		r = chk->data;
		chk->avail = chk->data + size;
#if ENABLE_MEM_DEBUG
		UPDATE_MEM_BLOCK_MAGIC(r, orig_size);
#endif

		buf->head.next = chk;
		buf->avail = chk;

#if ENABLE_MEM_INFO
		ALLOC_MEM_INFO(chk, r, orig_size, 1);

		if (chk->size > size)
			ALLOC_MEM_INFO(chk, chk->avail, chk->size - size, 0);
#endif
	} else {
		LHD;
		unsigned char *data = chk->data;
		unsigned char *avail = chk->avail;
		unsigned int chk_size = chk->size;
		debug("Data: 0x%lx, avail: 0x%lx, chk_size: 0x%x, size: 0x%x\n", (unsigned long)(intptr_t)data, (unsigned long)(intptr_t)avail, chk_size, size);
#if ENABLE_MEM_INFO
		MemInfo mi;

		// search unused mem info
		if (size >= sizeof(struct mem_info) * 2 && (mi = search_unused_mem_info(buf, size))) {
			unsigned int mi_orig_size = mi->size;
			r = mi->addr;
			mi->used = 1;
			mi->size = orig_size;
#if ENABLE_MEM_DEBUG
			UPDATE_MEM_BLOCK_MAGIC(r, orig_size);
			mi->addr = r;
#endif

			if (mi_orig_size > size) {
				MemInfo mi1, mi2;
				mi1 = alloc_mem_info(r, mi_orig_size - size, 0, mi->chunk, NULL, NULL);
				assert(mi1);
				
				mi2 = mi->next;
				mi->next = mi1;
				mi1->prev = mi;
				mi1->next = mi2;
				mi2->prev = mi1;
				chk->active_meminfo = mi1;
			}
		} else
#endif
		if (data + chk_size - avail >= size) {
			LHD;
#if ENABLE_MEM_INFO
			MemInfo active_meminfo;
			unsigned remain_size;
#endif

			r = chk->avail;
			chk->avail += size;
#if ENABLE_MEM_DEBUG
		UPDATE_MEM_BLOCK_MAGIC(r, orig_size);
#endif

#if ENABLE_MEM_INFO
			active_meminfo = chk->active_meminfo;
			active_meminfo->used = 1;
			active_meminfo->size = size;

			remain_size = data + chk_size - chk->avail;
			if (remain_size > 0)
				ALLOC_MEM_INFO(chk, chk->avail, remain_size, 0);
#endif
		} else {
			LHD;
			chk = alloc_mem_chunk(size);
			assert(chk);
			r = chk->data;
			chk->avail = chk->data + size;

#if ENABLE_MEM_DEBUG
			UPDATE_MEM_BLOCK_MAGIC(r, orig_size);
#endif

			buf->avail->next = chk;
			buf->avail = chk;

#if ENABLE_MEM_INFO
			ALLOC_MEM_INFO(chk, r, orig_size, 1);

			if (chk->size > size)
				ALLOC_MEM_INFO(chk, chk->avail, chk->size - size, 0);
#endif
		}
	}

	return r;
}

#if ENABLE_MEM_INFO
MemInfo search_unused_mem_info(MemBuf buf, int size)
{
	MemInfo mi = buf->meminfo.next;

	while (mi) {
		if (mi->used == 0 && mi->size >= size)
			return mi;
		mi = mi->next;
	}
	return NULL;
}
#endif

MemChunk alloc_mem_chunk(int size)
{
	MemChunk chk;

#if ENABLE_MEM_DEBUG
	size += CHK_MAGIC_SIZE;
#endif

	size = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

	chk = (MemChunk)mcc_malloc(sizeof(struct mem_chunk));
	if (chk == NULL) {
		LHD;
		return NULL;
	}

	chk->data = (unsigned char *)mcc_malloc(size);
	if (chk->data == NULL) {
		LHD;
		mcc_free(chk);
		return NULL;
	}
	chk->avail = chk->data;
	chk->size = size;
	chk->next = NULL;

#if ENABLE_MEM_DEBUG
	chk->size -= CHK_MAGIC_SIZE;
	UPDATE_CHK_MAGIC(chk->data, chk->size);
#endif
	return chk;
}

#if ENABLE_MEM_INFO
MemInfo alloc_mem_info(void *addr, int size, int used, MemChunk mc, MemInfo prev, MemInfo next)
{
	MemInfo mi;

	mi = (MemInfo)mcc_malloc(sizeof(struct mem_info));
	if (mi == NULL)
		return NULL;

	mi->addr = addr;
	mi->size = size;
	mi->chunk = mc;
	mi->used = used;
	mi->prev = prev;
	mi->next = next;

	return mi;
}
#endif

// free memory buffer
void free_mem_buf(MemBuf buf)
{
	MemChunk chk = buf->head.next;
#if ENABLE_MEM_INFO
	MemInfo mi = buf->meminfo.next;
#endif

	while (chk) {
		MemChunk next = chk->next;
#if ENABLE_MEM_DEBUG
		chk->data -= CHK_MAGIC_PREFIX_SIZE;
#endif
		mcc_free(chk->data);
		mcc_free(chk);
		chk = next;
	}
	buf->head.next = NULL;
	buf->avail = NULL;

#if ENABLE_MEM_INFO
	while (mi) {
		MemInfo next = mi->next;
		mcc_free(mi);
		mi = next;
	}
	buf->meminfo.prev = NULL;
	buf->meminfo.next = NULL;
#endif
}

unsigned int get_mem_buf_total_size(MemBuf buf)
{
	unsigned int size = 0;
	MemChunk mc = &buf->head;

	mc = mc->next;
	while (mc) {
		size += mc->size;
		mc = mc->next;
	}
	return size;
}

#if ENABLE_MEM_INFO
unsigned int get_mem_buf_used_size(MemBuf buf)
{
	unsigned int used_size = 0;
	MemInfo mi = &buf->meminfo;

	mi = mi->next;
	while (mi) {
		if (mi->used) {
			used_size += mi->size;
#if ENABLE_MEM_DEBUG
			used_size += CHK_MEM_BLOCK_MAGIC_SIZE;
#endif
		}
		mi = mi->next;
	}
	return used_size;
}

unsigned int get_mem_buf_unused_size(MemBuf buf)
{
	unsigned int unused_size = 0;
	MemInfo mi = &buf->meminfo;

	mi = mi->next;
	while (mi) {
		if (!mi->used)
			unused_size += mi->size;
		mi = mi->next;
	}
	return unused_size;
}

void dump_mem_info(MemBuf buf)
{
	MemInfo mi = &buf->meminfo;
	MemInfo p = mi->next;

	debug("  MemInfo(Head: %p, prev: %p, next: %p): %s.\n", mi, mi->prev, mi->next,
		  p == NULL ? "NULL" : "not NULL");

	while(p) {
		debug("	MemInfo(%p) addr(%p), size(%u), used(%d), prev(%p), next(%p), chunk(%p), magic_pre:(%x), magic_suffix(%x)\n",
			  p, p->addr, p->size, p->used, p->prev, p->next, p->chunk, GET_MEM_BLOCK_MAGIC_PREFIX(p->addr), GET_MEM_BLOCK_MAGIC_SUFFIX(p->addr, p->size));
		p = p->next;
	}
}
#endif

void dump_mem_buf(MemBuf buf)
{
	MemChunk head = &buf->head;
	MemChunk chk = head->next;

	if (!chk)
		debug("MemChunk: NULL.\n");

	while(chk) {
		debug("MemChunk(%p) start(%p), size(%u), avail(%p), next(%p), magic_pre:(%x), magic_suffix(%x)",
			  chk, chk->data, chk->size, chk->avail, chk->next, GET_CHK_MAGIC_PREFIX(chk), GET_CHK_MAGIC_SUFFIX(chk));
#if ENABLE_MEM_INFO
		debug_nofl(", active_meminfo: (%p)", chk->active_meminfo);
#endif
		debug_nofl("\n");
		chk = chk->next;
	}
	if (buf->avail) {
		MemChunk chk = buf->avail;
		debug("\tAvailable MemChunk(%p) start(%p), size(%u), avail(%p), next(%p), magic_pre:(%x), magic_suffix(%x)",
			  chk, chk->data, chk->size, chk->avail, chk->next, GET_CHK_MAGIC_PREFIX(chk), GET_CHK_MAGIC_SUFFIX(chk));
#if ENABLE_MEM_INFO
		debug_nofl(", active_meminfo: (%p)", chk->active_meminfo);
#endif
		debug_nofl("\n");
	}

#if ENABLE_MEM_INFO
	dump_mem_info(buf);
#endif
}

__END_DECLS