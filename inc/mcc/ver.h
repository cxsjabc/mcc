#ifndef VER_H
#define VER_H

#include "mcc/mcc_base.h"

#if CONFIG_VER_FROM_FILE
#include "../../scripts/data/VER"
#else
#define MAJOR "1"
#define MINOR "0"
#endif

#define BUILD_VER_DATE __DATE__
#define BUILD_VER_TIME __TIME__

#define VERSION MAJOR "." MINOR

__BEGIN_DECLS

const char *gen_build_ver();

__END_DECLS

#endif