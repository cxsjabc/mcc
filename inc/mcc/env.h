#ifndef ENV_H
#define ENV_H

#include "mcc/mcc_base.h"

__BEGIN_DECLS

int check_build_environment();
void check_running_environment();
int os_prepare();

__END_DECLS

#endif