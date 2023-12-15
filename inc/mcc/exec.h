#ifndef EXEC_H
#define EXEC_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

#define GCC_NAME "gcc"
#define CL_NAME "cl"
#define CLANG_NAME "clang"
#define GPLUSPLUS_NAME "g++"

int preprocess(const char *file, const char *prog);

__END_DECLS

#endif