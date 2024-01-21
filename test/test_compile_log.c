#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/args.h"
#include "mcc/compile_log.h"

__BEGIN_DECLS

void test_compile_log()
{
	ARG_VAR_VERBOSED0 = 1;
	verbose("This line should be appeared.\n");

	ARG_VAR_VERBOSED0 = 0;
	verbose("This line shouldn't be appeared.\n");

	always("%s all pass.\n", __func__);
	return;
}

__END_DECLS