#ifndef MCC_ARCH_H
#define MCC_ARCH_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

#if defined(_WIN32) && defined(_MSC_VER)
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#endif

__END_DECLS

#endif