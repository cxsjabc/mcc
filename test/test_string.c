#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"

__BEGIN_DECLS

void test_string()
{
	PCstr p, p1;
    int r;

    p = alloc_cstr(128);
    assert(p != NULL);

    cstr_show(p);

    r = init_cstr(p, "hello world", 6);
    assert(r == ERR_NONE);
    cstr_show(p);

    p1 = alloc_with_cstr("1234567890", 11);
    assert(p1 != NULL);
    cstr_show(p1);

	return;
}

__END_DECLS