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

        chk->active_meminfo = mi;
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
        //dump_mem_buf(buf);
        chk->active_meminfo = mi;

        if (chk->size > size) {
            MemInfo mi1;
            mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->meminfo, NULL);
            assert(mi1);

            UPDATE_MEM_INFO_PREV_NEXT(mi1);
            //dump_mem_buf(buf);
            chk->active_meminfo = mi1;
        }
    } else {
        unsigned char *data = chk->data;
        unsigned char *avail = chk->avail;
        unsigned int chk_size = chk->size;
        // search unused mem info
        if ((mi = search_unused_mem_info(buf, size))) {
            unsigned int orig_size = mi->size;
            r = mi->addr;
            mi->used = 1;
            mi->size = size;
            
            if (orig_size > size) {
                MemInfo mi1, mi2;
                mi1 = alloc_mem_info(r + size, orig_size - size, 0, chk, &buf->meminfo, NULL);
                assert(mi1);
                
                mi2 = mi->next;
                mi->next = mi1;
                mi1->prev = mi;
                mi1->next = mi2;
                mi2->prev = mi1;
                //dump_mem_buf(buf);
                chk->active_meminfo = mi1;
            }
        } else if (data + chk_size - avail >= size) {
            MemInfo active_meminfo;
            unsigned remain_size;

            r = chk->avail;
            chk->avail += size;

            //mi = alloc_mem_info(r, size, 1, chk, &buf->meminfo, NULL);
            //assert(mi);
            //UPDATE_MEM_INFO_PREV_NEXT(mi);
            //dump_mem_buf(buf);
            active_meminfo = chk->active_meminfo;
            active_meminfo->used = 1;
            active_meminfo->size = size;

            remain_size = data + chk_size - chk->avail;
            if (remain_size > 0) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, remain_size, 0, chk, &buf->meminfo, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
                //dump_mem_buf(buf);
                chk->active_meminfo = mi1;
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
            //dump_mem_buf(buf);
            chk->active_meminfo = mi;

            if (chk->size > size) {
                MemInfo mi1;
                mi1 = alloc_mem_info(chk->avail, chk->size - size, 0, chk, &buf->meminfo, NULL);
                assert(mi1);

                UPDATE_MEM_INFO_PREV_NEXT(mi1);
                //dump_mem_buf(buf);
                chk->active_meminfo = mi1;
            }
        }
    }

    return r;
}

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
    MemInfo mi = buf->meminfo.next;

    while (chk) {
        MemChunk next = chk->next;
        free(chk->data);
        free(chk);
        chk = next;
    }
    buf->head.next = NULL;
    buf->avail = NULL;

    while (mi) {
        MemInfo next = mi->next;
        free(mi);
        mi = next;
    }
    buf->meminfo.prev = NULL;
    buf->meminfo.next = NULL;
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
        debug("MemChunk(%p) start(%p), size(%u), avail(%p), next(%p), active_meminfo: (%p)\n",
              chk, chk->data, chk->size, chk->avail, chk->next, chk->active_meminfo);
        chk = chk->next;
    }
    if (buf->avail) {
        MemChunk chk = buf->avail;
        debug("\tAvailable MemChunk(%p) start(%p), size(%u), avail(%p), next(%p), active_meminfo(%p)\n",
              chk, chk->data, chk->size, chk->avail, chk->next, chk->active_meminfo);
    }

    dump_mem_info(buf);
}

void dump_mem_info(MemBuf buf)
{
    MemInfo mi = &buf->meminfo;
    MemInfo p = mi->next;

    debug("  MemInfo(Head: %p, prev: %p, next: %p): %s.\n", mi, mi->prev, mi->next,
          p == NULL ? "NULL" : "not NULL");

    while(p) {
        debug("    MemInfo(%p) addr(%p), size(%u), used(%d), prev(%p), next(%p)\n",
              p, p->addr, p->size, p->used, p->prev, p->next);
        p = p->next;
    }
}