#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mcc/string.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/size.h"
#include "mcc/mem_buf.h"

MemBuf init_mem_buf(int size)
{
    MemBuf buf;
    MemChunk chk;

    buf = (MemBuf)malloc(sizeof(struct mem_buf));
    if (buf == NULL)
        return NULL;

    buf->avail = NULL;
    buf->head.data = NULL;
    buf->head.next = NULL;

    buf->meminfo.next = buf->meminfo.prev = NULL;

    if (size > 0) {
        MemInfo mi;
        chk = alloc_mem_chunk(size);
        assert(chk);

        buf->head.next = chk;
        buf->avail = chk;

        mi = alloc_mem_info(chk->data, size, 0, chk, &buf->meminfo, NULL);
        assert(mi);
        UPDATE_MEM_INFO_PREV_NEXT(mi);
    }

    return buf;
}

void *alloc_mem_buf(MemBuf buf, int size)
{
    MemChunk chk;
    void *r;
    MemInfo mi;

    chk = buf->avail;
    if (chk == NULL) {
        LHD;
        chk = alloc_mem_chunk(size);
        assert(chk);
        r = chk->data;
        chk->avail = chk->data + size;

        buf->head.next = chk;
        buf->avail = chk;

        mi = alloc_mem_info(chk->data, size, 1, chk, &buf->meminfo, NULL);
        assert(mi);
        UPDATE_MEM_INFO_PREV_NEXT(mi);
        dump_mem_buf(buf);

        if (chk->size > size) {
            MemInfo mi1;
            mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->meminfo, NULL);
            assert(mi1);

            UPDATE_MEM_INFO_PREV_NEXT(mi1);
            dump_mem_buf(buf);
        }
    } else {
        unsigned char *data = chk->data;
        unsigned char *avail = chk->avail;
        unsigned int chk_size = chk->size;
        if (data + chk_size - avail >= size) {
            LHD;
            r = chk->avail;
            chk->avail += size;

            mi = alloc_mem_info(r, size, 1, chk, &buf->meminfo, NULL);
            assert(mi);
            UPDATE_MEM_INFO_PREV_NEXT(mi);
            dump_mem_buf(buf);

            if (data + chk_size - avail > size) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->meminfo, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
                dump_mem_buf(buf);
            }
        } else {
            LHD;
            chk = alloc_mem_chunk(size);
            assert(chk);
            r = chk->data;
            chk->avail = chk->data + size;

            buf->avail->next = chk;
            buf->avail = chk;

            mi = alloc_mem_info(chk->data, size, 1, chk, &buf->meminfo, NULL);
            assert(mi);
            UPDATE_MEM_INFO_PREV_NEXT(mi);
            dump_mem_buf(buf);

            if (chk->size > size) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->meminfo, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
                dump_mem_buf(buf);
            }
        }
    }

    return r;
}

MemChunk alloc_mem_chunk(int size)
{
    MemChunk chk;

    size = (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    chk = (MemChunk)malloc(sizeof(struct mem_chunk));
    if (chk == NULL)
        return NULL;

    chk->data = (unsigned char *)malloc(size);
    if (chk->data == NULL) {
        free(chk);
        return NULL;
    }
    chk->size = size;
    chk->next = NULL;
    return chk;
}

MemInfo alloc_mem_info(void *addr, int size, int used, MemChunk mc, MemInfo prev, MemInfo next)
{
    MemInfo mi;

    mi = (MemInfo)malloc(sizeof(struct mem_info));
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

void free_mem_buf(MemBuf buf)
{
    MemChunk chk = buf->head.next;

    while (chk) {
        MemChunk next = chk->next;
        free(chk->data);
        free(chk);
        chk = next;
    }
    buf->head.next = NULL;
    buf->avail = NULL;
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

unsigned int get_mem_buf_used_size(MemBuf buf)
{
    unsigned int used_size = 0;
    MemInfo mi = &buf->meminfo;

    mi = mi->next;
    while (mi) {
        if (mi->used)
            used_size += mi->size;
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

void dump_mem_buf(MemBuf buf)
{
    MemChunk head = &buf->head;
    MemChunk chk = head->next;

    if (!chk)
        debug("MemChunk: NULL.\n");

    while(chk) {
        debug("MemChunk(%p) start(%p), size(%u), avail(%p), next(%p)\n", chk, chk->data, chk->size, chk->avail, chk->next);
        chk = chk->next;
    }
    if (buf->avail) {
        MemChunk chk = buf->avail;
        debug("\tAvailable MemChunk(%p) start(%p), size(%u), avail(%p), next(%p)\n", chk, chk->data, chk->size, chk->avail, chk->next);
    }

    dump_mem_info(buf);
}

void dump_mem_info(MemBuf buf)
{
    MemInfo mi = &buf->meminfo;
    MemInfo p = mi->next;

    debug("MemInfo(Head: %p, prev: %p, next: %p): %s.\n", mi, mi->prev, mi->next, p == NULL ? "NULL" : "not NULL");

    while(p) {
        debug("MemInfo(%p) addr(%p), size(%u), used(%d), prev(%p), next(%p)\n", p, p->addr, p->size, p->used, p->prev, p->next);
        p = p->next;
    }
}