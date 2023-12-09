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
	char *argv[] = {"-I", ".", "-I/usr/xxx", "3.c", "-L./lib", "-L", "abc", "-Iabc/d", "-I", "..", "1.c", "2.c"};
    int argc = ARRAY_SIZE(argv);
    MccState ms;

    ms = create_mcc_state();
    assert(ms);

    r = parse_args(argc, argv, ms);
    assert(r == ERR_NONE);

    assert(ms->include_paths->size == 4);
    assert(ms->src_files->size == 3);
    assert(ms->lib_paths->size == 2);

    assert(STR_EQL(INC_PATH(ms, 0), "."));
    assert(STR_EQL(INC_PATH(ms, 1), "/usr/xxx"));
    assert(STR_EQL(INC_PATH(ms, 2), "abc/d"));
    assert(STR_EQL(INC_PATH(ms, 3), ".."));

    assert(STR_EQL(LIB_PATH(ms, 0), "./lib"));
    assert(STR_EQL(LIB_PATH(ms, 1), "abc"));

    assert(STR_EQL(SRC_FILE(ms, 0), "3.c"));
    assert(STR_EQL(SRC_FILE(ms, 1), "1.c"));
    assert(STR_EQL(SRC_FILE(ms, 2), "2.c"));


    // dump_mcc_state(ms);
    destroy_mcc_state(ms);

    always("%s pass.\n", __func__);
	return;
}