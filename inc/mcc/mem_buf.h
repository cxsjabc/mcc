#ifndef MEM_BUF_H
#define MEM_BUF_H

#define PAGE_SIZE 4096

#define ENABLE_MEM_INFO 0
#define ENABLE_MEM_DEBUG 1

#if ENABLE_MEM_DEBUG
#define CHK_MAGIC_PREFIX    0xFEFF
#define CHK_MAGIC_SUFFIX    0xFFFE
#define CHK_MAGIC_PREFIX_SIZE 2
#define CHK_MAGIC_SUFFIX_SIZE 2
#define CHK_MAGIC_SIZE      4
#define CHK_MEM_BLOCK_MAGIC_PREFIX 0xDEAD
#define CHK_MEM_BLOCK_MAGIC_SUFFIX 0xDAED
#define CHK_MEM_BLOCK_MAGIC_PREFIX_SIZE 2
#define CHK_MEM_BLOCK_MAGIC_SUFFIX_SIZE 2
#define CHK_MEM_BLOCK_MAGIC_SIZE   4

#define UPDATE_CHK_MAGIC(addr, size) \
{ \
    *(unsigned short *)(addr) = CHK_MAGIC_PREFIX; \
    (addr) += CHK_MAGIC_PREFIX_SIZE; \
    *(unsigned short *)((unsigned char *)addr + size) = CHK_MAGIC_SUFFIX; \
}

#define GET_CHK_MAGIC_PREFIX(chk) \
    (*(unsigned short *)((unsigned char *)(((chk)->data) - CHK_MAGIC_PREFIX_SIZE)))
#define GET_CHK_MAGIC_SUFFIX(chk) \
    (*(unsigned short *)((unsigned char *)(((chk)->data) + ((chk)->size))))
#endif

struct mem_info;

typedef struct mem_chunk
{
    unsigned char *data; // pointer to data
    unsigned int size; // size of data
    unsigned char *avail;

    struct mem_chunk *next; // pointer to next chunk

#if ENABLE_MEM_INFO
    struct mem_info *active_meminfo;
#endif
} *MemChunk;

#if ENABLE_MEM_INFO
typedef struct mem_info
{
    unsigned char *addr;
    unsigned int size;
    unsigned int used;

    MemChunk chunk; // pointer to the chunk

    struct mem_info *prev;
    struct mem_info *next;
} *MemInfo;
#endif

typedef struct mem_buf
{
    MemChunk avail;
    struct mem_chunk head;

#if ENABLE_MEM_INFO
    struct mem_info meminfo;
#endif    
} *MemBuf;

MemBuf init_mem_buf(int size);
void *alloc_from_mem_buf(MemBuf buf, int size);
void free_mem_buf(MemBuf buf);
void dump_mem_buf(MemBuf buf);

MemChunk alloc_mem_chunk(int size);

#if ENABLE_MEM_INFO
MemInfo alloc_mem_info();
void dump_mem_info(MemBuf buf);

MemInfo search_unused_mem_info(MemBuf buf, int size);
#endif

unsigned int get_mem_buf_total_size(MemBuf buf);
#define BUF_TOTAL_SIZE(buf) get_mem_buf_total_size(buf)

#if ENABLE_MEM_INFO
unsigned int get_mem_buf_used_size(MemBuf buf);
unsigned int get_mem_buf_unused_size(MemBuf buf);

#define BUF_USED_SIZE(buf) get_mem_buf_used_size(buf)
#define BUF_UNUSED_SIZE(buf) get_mem_buf_unused_size(buf)

#define BUF_SIZE_MATCH(buf) (BUF_TOTAL_SIZE(buf) == BUF_USED_SIZE(buf) + BUF_UNUSED_SIZE(buf))
#define BUF_SIZE_DUMP(buf) { debug("MemBuf(%p), total size: %d, used size: %d, unused_size: %d\n", buf, BUF_TOTAL_SIZE(buf), BUF_USED_SIZE(buf), BUF_UNUSED_SIZE(buf)); }

#define UPDATE_MEM_INFO_PREV_NEXT(mi) \
    if (buf->meminfo.prev) { \
        buf->meminfo.prev->next = (mi); \
        (mi)->prev = buf->meminfo.prev; \
        buf->meminfo.prev = (mi); \
    } \
    else \
        buf->meminfo.prev = buf->meminfo.next = (mi), (mi)->prev = &buf->meminfo;

#define ALLOC_MEM_INFO(chk, addr, size, used) \
{ \
    MemInfo mi; \
    mi = alloc_mem_info((addr), (size), (used), (chk), NULL, NULL); \
    assert(mi); \
    UPDATE_MEM_INFO_PREV_NEXT(mi); \
    (chk)->active_meminfo = mi; \
}
#endif

#endif