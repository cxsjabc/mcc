#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/log.h"
#include "mcc/size.h"

__BEGIN_DECLS

void check_type_size()
{
	debug("sizeof __int16_t: %d\n", SIZEOF(int16_t));
	debug("sizeof __int32_t: %d\n", SIZEOF(int32_t));
	debug("sizeof __int64_t: %d\n", SIZEOF(int64_t));
}

__END_DECLS