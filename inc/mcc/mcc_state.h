#ifndef MCC_STATE_H
#define MCC_STATE_H

#include "mcc/array.h"
#include "mcc/file.h"
#include "mcc/mem.h"
#include "mcc/mem_buf.h"

extern struct mcc_state MS;

typedef struct mcc_state
{
    // mem buf
    struct mem_buf global_buf;  // buffers for all compiled files
    struct mem_buf compile_buf; // buffers for the current compiled file

    // header or lib pathes
    DynArray include_paths; // header file pathes
    DynArray lib_paths;     // library pathes

    DynArray src_files;     // files to compile
    DynArray obj_files;     // objects to compile

} *MccState;

void setup_mcc_state();
void clean_mcc_state();
void dump_mcc_state();

// if type is -1, get file type from path
int mcc_state_add_files(const char *path, FileType type);

#endif