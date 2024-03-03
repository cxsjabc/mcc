#ifndef EXE_H
#define EXE_H

#include "mcc/mcc_base.h"

#include "mcc/type.h"

#if defined(_WINDOWS) || defined(_MSC_VER)
#include "mcc/win/pe.h"
#else
#include "mcc/linux/elf.h"
#endif

#endif