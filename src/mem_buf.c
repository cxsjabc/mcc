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

    if (size > 0) {
        chk = alloc_mem_chunk(size);
        assert(chk);

        buf->head.next = chk;
        buf->avail = chk;
    }

    return buf;
}

void *alloc_mem_buf(MemBuf buf, int size)
{
    MemChunk chk;
    void *r;

    chk = buf->avail;
    if (chk == NULL) {
        LH;
        chk = alloc_mem_chunk(size);
        assert(chk);
        r = chk->data;
        buf->head.next = chk;
        buf->avail = chk;
    } else {
        unsigned char *data = chk->data;
        unsigned char *avail = chk->avail;
        unsigned int chk_size = chk->size;
        if (data + chk_size - avail >= size) {
            LH;
            r = chk->avail;
            chk->avail += size;
        } else {
            LH;
            chk = alloc_mem_chunk(size);
            assert(chk);
            r = chk->data;
            chk->avail = chk->data + size;
            buf->avail->next = chk;
        }
    }

    return r;
}

MemChunk alloc_mem_chunk(int size)
{
    MemChunk chk;

    size = (size + PAGE_SIZE) & ~(PAGE_SIZE - 1);

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

void dump_mem_buf(MemBuf buf)
{
    MemChunk head = &buf->head;
    LH;
    while(head->next) {
        LH;
        debug("MemChunk start(%p), size(%u), avail(%p), next(%p)\n", head->data, head->size, head->avail, head->next);
        head = head->next;
    }
    if (buf->avail) {
        MemChunk chk = buf->avail;
        debug("\tAvailable MemChunk start(%p), size(%u), avail(%p), next(%p)\n", chk->data, chk->size, chk->avail, chk->next);
    }
}