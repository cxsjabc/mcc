#define NO_DEBUG 0

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/array.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/mem_buf.h"
#include "mcc/size.h"

__BEGIN_DECLS

void test_mem_buf()
{
	MemBuf buf;
    void *p, *p1, *p2, *p3;

    buf = init_mem_buf(0);
    LHD;
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf);
    assert(buf && BUF_SIZE_MATCH(buf));
#endif
    //assert(CHK_MAGIC_MATCH(buf->avail));

    LHD;
    p = alloc_from_mem_buf(buf, 12);
    assert(p);
    memset(p, 0x2, 12);
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf);
    assert(BUF_SIZE_MATCH(buf));
#endif
    assert(CHK_MAGIC_MATCH(buf->avail));
    assert(CHK_MEM_BLOCK_MAGIC_MATCH(p, 12));

    LHD;
    p1 = alloc_from_mem_buf(buf, 4096);
    assert(p1);
    memset(p1, 0x3, 4096);
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf)
    assert(BUF_SIZE_MATCH(buf));
#endif
    assert(CHK_MAGIC_MATCH(buf->avail));
    assert(CHK_MEM_BLOCK_MAGIC_MATCH(p1, 4096));

    LHD;
    p2 = alloc_from_mem_buf(buf, 234);
    assert(p2);
    memset(p2, 0x4, 234);
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf)
    assert(p2 && BUF_SIZE_MATCH(buf));
#endif
    assert(CHK_MAGIC_MATCH(buf->avail));
    assert(CHK_MEM_BLOCK_MAGIC_MATCH(p2, 234));

    LHD;
    p3 = alloc_from_mem_buf(buf, 128);
    assert(p3);
    memset(p3, 0x5, 128);
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf)
    assert(p3 && BUF_SIZE_MATCH(buf));
#endif
    assert(CHK_MAGIC_MATCH(buf->avail));
    assert(CHK_MEM_BLOCK_MAGIC_MATCH(p3, 128));

    assert(p && p1 && p2 && p3);

    LHD;
    free_mem_buf(buf);
    dump_mem_buf(buf);
#if ENABLE_MEM_INFO
    BUF_SIZE_DUMP(buf)
    assert(BUF_SIZE_MATCH(buf));
#endif

    always("%s all pass.\n", __func__);
	return;
}

__END_DECLS