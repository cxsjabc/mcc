//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/log.h"
#include "test.h"

__BEGIN_DECLS

int main(int argc, char *argv[])
{
	always("Test starting.\n");

#if ENABLE_TEST_TYPE
    test_type();
#endif

#if ENABLE_TEST_STRING
    test_string();
#endif

#if ENABLE_TEST_FILE
    test_file();
#endif    
	
#if ENABLE_TEST_KEYWORD
    test_keyword();
#endif

#if ENABLE_TEST_ID
    test_id();
#endif

#if ENABLE_TEST_TOKEN
    test_token();
#endif

#if ENABLE_TEST_MEM_BUF
    test_mem_buf();
#endif

#if ENABLE_TEST_ARRAY
    test_array();
#endif

#if ENABLE_TEST_ARGS
    test_args();
#endif

    always("Test end.\n");
	return 0;
}

__END_DECLS