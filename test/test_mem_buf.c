#define NO_DEBUG 1

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
    assert(buf);

    p = alloc_mem_buf(buf, 12);
    LHD;
    dump_mem_buf(buf);
    assert(p);
    LHD;

    p1 = alloc_mem_buf(buf, 4096);
    dump_mem_buf(buf);
    assert(p1);
    LHD;

    p2 = alloc_mem_buf(buf, 234);
    dump_mem_buf(buf);
    assert(p2);
    LHD;

    p3 = alloc_mem_buf(buf, 128);
    dump_mem_buf(buf);
    assert(p3);
    LHD;

    dump_mem_buf(buf);

    // always("%s all pass.\n", __func__);
	return;
}