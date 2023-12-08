#ifndef MCC_STATE_H
#define MCC_STATE_H

#include "mcc/array.h"
#include "mcc/file.h"

extern struct mcc_state MS;

typedef struct mcc_state
{
    DynArray include_paths; // header file pathes
    DynArray lib_paths;     // library pathes

    DynArray src_files;     // files to compile
    DynArray obj_files;     // objects to compile

} *MccState;

void setup_mcc_state();
void dump_mcc_state();

// if type is -1, get file type from path
int mcc_state_add_files(const char *path, FileType type);

#endif