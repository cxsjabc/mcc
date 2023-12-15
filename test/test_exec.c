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
	preprocess("./scripts/data/future/hello.c", CLANG_NAME);
#elif defined(__GNUC__)
	info("Compiling with __GNUC__\n");
	preprocess("./scripts/data/future/hello.c", GCC_NAME);
#elif defined(_WIN32)
	info("Compiling with _WIN32\n");
	preprocess("./scripts/data/future/hello.c", CL_NAME);
#else
#error "Unknown compiler"
#endif
	return;
}

__END_DECLS