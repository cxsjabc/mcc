//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/log.h"
#include "test.h"

int main(int argc, char *argv[])
{
	always("Test starting...\n");

#if ENABLE_TEST_TYPE
    test_type();
#endif

#if ENABLE_TEST_STRING
    test_string();
#endif

#if ENABLE_TEST_FILE
    test_file();
#endif    
	
	return 0;
}