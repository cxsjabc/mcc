#include <stdio.h>

#include "mcc/error.h"
#include "mcc/mcc_state.h"

struct mcc_state MS;

void setup_mcc_state()
{
    MS.include_paths = create_dynamic_array(32);
    assert(MS.include_paths);

    MS.lib_paths = create_dynamic_array(0);
    assert(MS.lib_paths);

    MS.src_paths = create_dynamic_array(2);
    assert(MS.src_paths);
}

int mcc_state_add_files(const char *path, FileType type)
{
    int r = ERR_FAIL;

    switch (type)
    {
    case FILE_TYPE_C:
        r = dynamic_array_push(MS.src_paths, path);
        break;
    case FILE_TYPE_H:
        r = dynamic_array_push(MS.include_paths, path);
        break;
    case FILE_TYPE_O:
        r = dynamic_array_push(MS.lib_patha, path);
        break;
    default:
        error("Unknown file type, %d\n", type);
        break;
    }

    return r;
}