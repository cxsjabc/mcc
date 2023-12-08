#include <assert.h>
#include <stdio.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"

struct mcc_state MS;

void setup_mcc_state()
{
    MS.include_paths = create_dynamic_array(32);
    MS.include_paths->to_string = NULL;
    assert(MS.include_paths);

    MS.lib_paths = create_dynamic_array(0);
    assert(MS.lib_paths);

    MS.src_files = create_dynamic_array(2);
    assert(MS.src_files);
}

void dump_mcc_state()
{
    dump_dynamic_array(MS.include_paths);
    dump_dynamic_array(MS.lib_paths);
    dump_dynamic_array(MS.src_files);
}

int mcc_state_add_files(const char *path, FileType type)
{
    int r = ERR_FAIL;

    if (type == FILE_TYPE_UNKNOWN)
        type = get_file_type(path);

    switch (type)
    {
    case FILE_TYPE_C:
        r = dynamic_array_push(MS.src_files, (void *)path);
        break;
    case FILE_TYPE_HEADER_PATH:
        r = dynamic_array_push(MS.include_paths, (void *)path);
        break;
    case FILE_TYPE_OBJECT:
        r = dynamic_array_push(MS.obj_files, (void *)path);
        break;
    case FILE_TYPE_LIB_PATH:
        r = dynamic_array_push(MS.lib_paths, (void *)path);
        break;
    default:
        error("Unknown file type, %d\n", type);
        break;
    }

    return r;
}