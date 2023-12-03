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
        LHD;
        chk = alloc_mem_chunk(size);
        assert(chk);
        r = chk->data;
        chk->avail = chk->data + size;

        buf->head.next = chk;
        buf->avail = chk;
    } else {
        unsigned char *data = chk->data;
        unsigned char *avail = chk->avail;
        unsigned int chk_size = chk->size;
        if (data + chk_size - avail >= size) {
            LHD;
            r = chk->avail;
            chk->avail += size;
        } else {
            LHD;
            chk = alloc_mem_chunk(size);
            assert(chk);
            r = chk->data;
            chk->avail = chk->data + size;

            buf->avail->next = chk;
            buf->avail = chk;
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
}