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
    struct mem_info meminfo;
} *MemBuf;

MemBuf init_mem_buf(int size);
void *alloc_mem_buf(MemBuf buf, int size);
void free_mem_buf(MemBuf buf);
void dump_mem_buf(MemBuf buf);

MemChunk alloc_mem_chunk(int size);

MemInfo alloc_mem_info();
void dump_mem_info(MemBuf buf);

unsigned int get_mem_buf_total_size(MemBuf buf);
unsigned int get_mem_buf_used_size(MemBuf buf);
unsigned int get_mem_buf_unused_size(MemBuf buf);

#define BUF_TOTAL_SIZE(buf) get_mem_buf_total_size(buf)
#define BUF_USED_SIZE(buf) get_mem_buf_used_size(buf)
#define BUF_UNUSED_SIZE(buf) get_mem_buf_unused_size(buf)

#define BUF_SIZE_MATCH(buf) (BUF_TOTAL_SIZE(buf) == BUF_USED_SIZE(buf) + BUF_UNUSED_SIZE(buf))
#define BUF_SIZE_DUMP(buf) { debug("MemBuf(%p), total size: %d, used size: %d, unused_size: %d\n", buf, BUF_TOTAL_SIZE(buf), BUF_USED_SIZE(buf), BUF_UNUSED_SIZE(buf)); }

#define UPDATE_MEM_INFO_PREV_NEXT(mi) \
    if (buf->meminfo.prev) { \
        buf->meminfo.prev->next = mi; \
        mi->prev = buf->meminfo.prev; \
        buf->meminfo.prev = mi; \
    } \
    else \
        buf->meminfo.prev = buf->meminfo.next = mi;

#endif