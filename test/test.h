#ifndef TEST_H
#define TEST_H

#include "mcc/mcc_base.h"

#define ENABLE_TEST_TYPE 0
#define ENABLE_TEST_STRING 0
#define ENABLE_TEST_FILE 0
#define ENABLE_TEST_KEYWORD 0
#define ENABLE_TEST_ID 0
#define ENABLE_TEST_TOKEN 0
#define ENABLE_TEST_MEM_BUF 0
#define ENABLE_TEST_ARRAY 1
#define ENABLE_TEST_ARGS 1

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

__END_DECLS

#endif