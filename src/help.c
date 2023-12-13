#include <stdio.h>

#include "mcc/help.h"

__BEGIN_DECLS

void show_help()
{
	printf("Usage: mcc [options] [file]\n");
	printf("Options:\n");
	printf("  -h, --help       Show this help message and exit\n");
	printf("  -v, --version    Show version number and exit\n");
	printf("  -b, --build      Show build version\n");
}

__END_DECLS