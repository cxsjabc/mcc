#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/array.h"
#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"

char *char_str_to_string(void *data)
{
    char *p = (char *)data;
    return p;
}

void test_array()
{
	DynArray arr;
    char *s;
    int i, r;

    arr = create_dynamic_array(2);
    assert(arr);

    //arr->compare = strcmp;
    arr->destroy = free;
    arr->to_string = char_str_to_string;

    for (i = 0; i < 20; ++i) {
        s = malloc(sizeof(char) * 16);
        assert(s);
        sprintf(s, "string %d", i);

        r = dynamic_array_push(arr, s);
        assert(r > 0);
    }

    dump_dynamic_array(arr);

    destroy_dynamic_array(arr);
    dump_dynamic_array(arr);

	return;
}