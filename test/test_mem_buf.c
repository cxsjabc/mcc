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

void test_mem_buf()
{
	MemBuf buf;
    void *p, *p1, *p2, *p3;

    buf = init_mem_buf(0);
    LHD;
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf);
    assert(buf && BUF_SIZE_MATCH(buf));

    LHD;
    p = alloc_mem_buf(buf, 12);
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf);
    assert(p && BUF_SIZE_MATCH(buf));

    LHD;
    p1 = alloc_mem_buf(buf, 4096);
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf)
    assert(p1 && BUF_SIZE_MATCH(buf));

    LHD;
    p2 = alloc_mem_buf(buf, 234);
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf)
    assert(p2 && BUF_SIZE_MATCH(buf));
    
    LHD;
    p3 = alloc_mem_buf(buf, 128);
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf)
    assert(p3 && BUF_SIZE_MATCH(buf));

    LHD;
    free_mem_buf(buf);
    dump_mem_buf(buf);
    BUF_SIZE_DUMP(buf)
    assert(BUF_SIZE_MATCH(buf));

    always("%s all pass.\n", __func__);
	return;
}