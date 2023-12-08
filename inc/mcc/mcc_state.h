#ifndef MCC_STATE_H
#define MCC_STATE_H

#include "mcc/array.h"

extern struct mcc_state MS;

typedef struct mcc_state
{
    DynArray include_paths;
    DynArray lib_paths;       // library paths
    DynArray src_paths;

} *MccState;

void setup_mcc_state();
int mcc_state_add_files(const char *path, FileType type);

#endif