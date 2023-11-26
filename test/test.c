//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/log.h"
#include "test.h"

int main(int argc, char *argv[])
{
	always("Test starting...\n");
    test_type();
    test_string();
	
	return 0;
}