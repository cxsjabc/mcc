#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/exec.h"
#include "mcc/log.h"

__BEGIN_DECLS

void test_exec()
{
#if defined(__clang__)
	info("Compiling with clang\n");
	preprocess("1.c", CLANG_NAME);
#elif defined(__GNUC__)
	info("Compiling with __GNUC__\n");
	preprocess("1.c", GCC_NAME);
#elif defined(_MSC_VER)
	info("Compiling with msvc cl.exe\n");
	preprocess("1.c", CL_NAME);
#else
#error "Unknown compiler"
#endif
	return;
}

__END_DECLS