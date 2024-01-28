#include <stdarg.h>
#include <stdio.h>

#include "mcc/compile_log.h"

__BEGIN_DECLS

#if 0
void _expect(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	cerror(EXPECT " " fmt, __VA_ARGS__);
	va_end(ap);
}

void expect(const char *s)
{
	_expect(EXPECT " ", s);
}
#endif

__END_DECLS