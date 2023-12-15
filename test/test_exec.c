#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/exec.h"

__BEGIN_DECLS

void test_exec()
{
	preprocess("./scripts/data/future/hello.c");
	return;
}

__END_DECLS