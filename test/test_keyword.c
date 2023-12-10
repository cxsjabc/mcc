#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/array.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/keyword.h"
#include "mcc/size.h"

__BEGIN_DECLS

void test_keyword()
{
	const char *arr[] = { "int", "ino", "case", "break", "break1" };
	int res_arr[5];
	int right_arr[] = { 1, 0, 1, 1, 0 };
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(arr); i++) {
		res_arr[i] = is_keyword(arr[i]);
	}

	assert(is_array_equal(res_arr, right_arr, ARRAY_SIZE(right_arr)));

	always("%s all pass.\n", __func__);
	return;
}

__END_DECLS