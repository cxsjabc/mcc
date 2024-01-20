//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/env.h"
#include "mcc/id.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mem_buf.h"
#include "test.h"

__BEGIN_DECLS

void test_prepare()
{
	check_build_environment();
	check_running_environment();

	setup_global_mem_buf();

	init_char_type_table();
	lex_init();
}

void test_postprocessing()
{
	uninit_global_mem_buf();
}

int main(int argc, char *argv[])
{
	always("Test starting.\n");

	test_prepare();

#if ENABLE_TEST_ARGS
	test_args();
#endif

#if ENABLE_TEST_ARRAY
	test_array();
#endif

#if ENABLE_TEST_COMPILE_LOG
	test_compile_log();
#endif

#if ENABLE_TEST_EXEC
	test_exec();
#endif

#if ENABLE_TEST_FILE
	test_file();
#endif	

#if ENABLE_TEST_HASH
	test_hash();
#endif

#if ENABLE_TEST_ID
	test_id();
#endif

#if ENABLE_TEST_KEYWORD
	test_keyword();
#endif

#if ENABLE_TEST_LEX
	test_lex();
#endif

#if ENABLE_TEST_MCC_STATE
	test_mcc_state();
#endif

#if ENABLE_TEST_MEM_BUF
	test_mem_buf();
#endif

#if ENABLE_TEST_SIZE
	test_size();
#endif

#if ENABLE_TEST_STRING
	test_string();
#endif

#if ENABLE_TEST_TOKEN
	test_token();
#endif

#if ENABLE_TEST_TYPE
	test_type();
#endif

#if ENABLE_TEST_STACK
	test_stack();
#endif

	test_postprocessing();

	always("Test end.\n");
	return 0;
}

__END_DECLS