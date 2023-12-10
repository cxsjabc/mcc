#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/log.h"
#include "mcc/mem.h"
#include "mcc/string.h"
#include "mcc/print.h"

__BEGIN_DECLS

void print_pointer(void *p)
{
#if defined(__amd64__) || defined(_WIN64)
    debug_nofl("%#lx", (unsigned long)(intptr_t)(p));
#elif defined(__i386__) || defined(_WIN32)
    debug_nofl("%#x", (intptr_t)(p));
#else
#error "Unknown architecture"
#endif
}

__END_DECLS