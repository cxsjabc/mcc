#define NO_DEBUG 0

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

__BEGIN_DECLS

void test_args_include_lib_src();
void test_args1();

void test_args()
{
	test_args_include_lib_src();
	test_args1();
}

void test_args_include_lib_src()
{
	int r;
	const char *argv[] = {"-I", ".", "-I/usr/xxx", "3.c", "-L./lib", "-L", "abc", "-Iabc/d", "-I", "..", "1.c", "2.c"};
	int argc = ARRAY_SIZE(argv);
	MccState ms;

	ms = create_mcc_state();
	assert(ms);

	r = parse_args(argc, (char **)argv, ms);
	assert(r >= OK);

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

void test_args1()
{
	int r;
	char *argv1[] = { "-c", "1.c", "2.c", "-o", "main" };
	char *argv2[] = { "1.c", "2.c", "-o", "main.exe" };

	r = parse_args(asize(argv1), (char **)argv1, NULL);
	assert(r != OK);
	args_clean_state(), clean_mcc_state(NULL);

	setup_mcc_state(NULL);
	r = parse_args(asize(argv2), (char **)argv2, NULL);
	assert(r >= OK);
	args_clean_state(), clean_mcc_state(NULL);

	always("%s pass.\n", __func__);
	return;
}

__END_DECLS