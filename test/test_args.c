#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/args.h"
#include "mcc/array.h"
#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/size.h"
#include "mcc/string.h"

void test_args()
{
    int r;
	char *argv[] = {"-I", ".", "-I/usr/xxx", "-L./lib", "-L", "abc", "-Iabc/d", "-I", "..", "1.c", "2.c"};
    int argc = ARRAY_SIZE(argv);
    MccState ms;

    ms = create_mcc_state();
    assert(ms);

    r = parse_args(argc, argv, ms);
    assert(r == ERR_NONE);

    dump_mcc_state(ms);
    clean_mcc_state(ms);

    always("%s pass.\n", __func__);
	return;
}