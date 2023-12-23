#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/array.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/id.h"
#include "mcc/size.h"

__BEGIN_DECLS

void test_char_table()
{
	char table[] = { 'a', 'b', 'c', 'd', 'e', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '_', '-' };
	int size = ARRAY_SIZE(table);
	int i;

	for (i = 0; i < size; i++) {
		CHAR_TYPE t = get_char_type(table[i]);
		always("char %c(%d) type: %s\n", table[i], (int)table[i], get_char_type_str(t));
	}
}

void test_id()
{
	const char *arr[] = { "int", "i1", "1abc", "_ac12", "_2b34", "abc", "123" };
	int res_arr[128];
	int right_arr[] = { 1, 1, 0, 1, 1, 1, 0 };
	unsigned int i;

	init_char_type_table();

	for (i = 0; i < ARRAY_SIZE(arr); i++) {
		res_arr[i] = is_identifier(arr[i]);
	}

	assert(is_array_equal(res_arr, right_arr, ARRAY_SIZE(right_arr)));

	test_char_table();

	always("%s all pass.\n", __func__);
	return;
}

__END_DECLS