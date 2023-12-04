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

    buf->info.next = buf->info.prev = NULL;

    if (size > 0) {
        MemInfo mi;
        chk = alloc_mem_chunk(size);
        assert(chk);

        buf->head.next = chk;
        buf->avail = chk;

        mi = alloc_mem_info(chk->data, size, 0, chk, &buf->info, NULL);
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

        mi = alloc_mem_info(chk->data, size, 1, chk, &buf->info, NULL);
        assert(mi);
        UPDATE_MEM_INFO_PREV_NEXT(mi);

        if (chk->size > size) {
            MemInfo mi1;
            mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->info, NULL);
            assert(mi1);

            UPDATE_MEM_INFO_PREV_NEXT(mi1);
        }
    } else {
        unsigned char *data = chk->data;
        unsigned char *avail = chk->avail;
        unsigned int chk_size = chk->size;
        if (data + chk_size - avail >= size) {
            LHD;
            r = chk->avail;
            chk->avail += size;

            mi = alloc_mem_info(r, size, 1, chk, &buf->info, NULL);
            assert(mi);
            UPDATE_MEM_INFO_PREV_NEXT(mi);

            if (data + chk_size - avail > size) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->info, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
            }
        } else {
            LHD;
            chk = alloc_mem_chunk(size);
            assert(chk);
            r = chk->data;
            chk->avail = chk->data + size;

            buf->avail->next = chk;
            buf->avail = chk;

            mi = alloc_mem_info(chk->data, size, 1, chk, &buf->info, NULL);
            assert(mi);
            UPDATE_MEM_INFO_PREV_NEXT(mi);

            if (chk->size > size) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->info, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
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

void dump_mem_buf(MemBuf buf)
{
    MemChunk head = &buf->head;
    MemChunk chk = head->next;

    if (!chk)
        debug("MemChunk: NULL.\n");

    while(chk) {
        LHD;
        debug("MemChunk start(%p), size(%u), avail(%p), next(%p)\n", chk->data, chk->size, chk->avail, chk->next);
        chk = chk->next;
    }
    if (buf->avail) {
        MemChunk chk = buf->avail;
        debug("\tAvailable MemChunk start(%p), size(%u), avail(%p), next(%p)\n", chk->data, chk->size, chk->avail, chk->next);
    }

    dump_mem_info(buf);
}

void dump_mem_info(MemBuf buf)
{
    MemInfo mi = &buf->info;
    MemInfo p = mi->next;

    if (!p)
        debug("MemInfo: NULL.\n");

    while(p) {
        LHD;
        debug("MemInfo addr(%p), size(%u), used(%d), prev(%p), next(%p)\n", p->addr, p->size, p->used, p->prev, p->next);
        p = p->next;
    }
}