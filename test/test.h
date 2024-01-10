#ifndef TEST_H
#define TEST_H

#include "mcc/mcc_base.h"

#define ENABLE_TEST_ARGS 0
#define ENABLE_TEST_ARRAY 0
#define ENABLE_TEST_COMPILE_LOG 0
#define ENABLE_TEST_EXEC 0
#define ENABLE_TEST_FILE 0
#define ENABLE_TEST_HASH 0
#define ENABLE_TEST_ID 0
#define ENABLE_TEST_KEYWORD 0
#define ENABLE_TEST_LEX 0
#define ENABLE_TEST_MCC_STATE 0
#define ENABLE_TEST_MEM_BUF 0
#define ENABLE_TEST_SIZE 0
#define ENABLE_TEST_STRING 0
#define ENABLE_TEST_TOKEN 0
#define ENABLE_TEST_TYPE 0
#define ENABLE_TEST_STACK 1

#define TEST_BEGIN always("Test %s begin.\n", __func__)
#define TEST_END always("Test %s end.\n", __func__)

__BEGIN_DECLS

void test_args();
void test_array();
void test_compile_log();
void test_exec();
void test_file();
void test_hash();
void test_id();
void test_keyword();
void test_lex();
void test_mcc_state();
void test_mem_buf();
void test_size();
void test_string();
void test_token();
void test_type();
void test_stack();

__END_DECLS

#endif