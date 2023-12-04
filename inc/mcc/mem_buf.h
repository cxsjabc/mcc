#ifndef MEM_BUF_H
#define MEM_BUF_H

#define PAGE_SIZE 4096

typedef struct mem_chunk
{
    unsigned char *data; // pointer to data
    unsigned int size; // size of data
    unsigned char *avail;
    struct mem_chunk *next; // pointer to next chunk
} *MemChunk;

typedef struct mem_info
{
    unsigned char *addr;
    unsigned int size;
    unsigned int used;

    MemChunk chunk; // pointer to the chunk

    struct mem_info *prev;
    struct mem_info *next;
} *MemInfo;

typedef struct mem_buf
{
    MemChunk avail;
    struct mem_chunk head;
    struct mem_info info;
} *MemBuf;

MemBuf init_mem_buf(int size);
void *alloc_mem_buf(MemBuf buf, int size);
void free_mem_buf(MemBuf buf);
void dump_mem_buf(MemBuf buf);

MemChunk alloc_mem_chunk(int size);

MemInfo alloc_mem_info();
void dump_mem_info(MemBuf buf);

#define UPDATE_MEM_INFO_PREV_NEXT(mi) \
    if (buf->info.prev) { \
        buf->info.prev->next = mi; \
        mi->prev = buf->info.prev; \
        buf->info.prev = mi; \
    } \
    else \
        buf->info.prev = buf->info.next = mi;

#endif