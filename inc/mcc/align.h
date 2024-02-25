#ifndef ALIGN_H
#define ALIGN_H

#include "mcc/mcc_base.h"

#define ALIGN(addr, align) (((addr) + (align) - 1) & ~((align) - 1))

__BEGIN_DECLS

__END_DECLS

#endif
