#define NO_DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/string.h"
#include "mcc/log.h"
#include "mcc/error.h"
#include "mcc/file.h"

void test_file()
{
	const char *f1 = "./test1.c", *f2 = "/test2.cpp", *f3 = "test3.asm";
    const char *ext, *file;
    int r;

    ext = get_file_extension(f1);
    assert(strcmp(ext, "c") == 0);
    ext = get_file_extension(f2);
    assert(strcmp(ext, "cpp") == 0);

    r = is_absolute_path(f1);
    assert(r == 0);
    r = is_absolute_path(f2);
    assert(r == 1);

    file = get_file_name(f1);
    assert(strcmp(file, "test1.c") == 0);
    file = get_file_name(f2);
    assert(strcmp(file, "test2.cpp") == 0);
    file = get_file_name(f3);
    assert(strcmp(file, "test3.asm") == 0);

	return;
}