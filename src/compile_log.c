#include <stdio.h>

#include "mcc/compile_log.h"

__BEGIN_DECLS

int is_verbose_enabled;

void set_verbose_enabled(int enabled)
{
	is_verbose_enabled = enabled;
}

__END_DECLS