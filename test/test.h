#ifndef TEST_H
#define TEST_H

#include "mcc/mcc_base.h"

#define ENABLE_TEST_TYPE 0
#define ENABLE_TEST_STRING 0
#define ENABLE_TEST_FILE 0
#define ENABLE_TEST_KEYWORD 0
#define ENABLE_TEST_ID 1
#define ENABLE_TEST_TOKEN 0
#define ENABLE_TEST_MEM_BUF 0
#define ENABLE_TEST_ARRAY 0
#define ENABLE_TEST_ARGS 0
#define ENABLE_TEST_COMPILE_LOG 0
#define ENABLE_TEST_EXEC 0
#define ENABLE_TEST_MCC_STATE 0
#define ENABLE_TEST_SIZE 0
#define ENABLE_TEST_HASH 0

__BEGIN_DECLS

void test_type();
void test_string();
void test_file();
void test_keyword();
void test_id();
void test_token();
void test_mem_buf();
void test_array();
void test_args();
void test_compile_log();
void test_exec();
void test_mcc_state();
void test_size();
void test_hash();

__END_DECLS

#endif