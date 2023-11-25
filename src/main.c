#include <stdio.h>

#include "mcc/ver.h"
#include "mcc/args.h"
#include "mcc/help.h"

int main(int argc, char *argv[])
{
	printf("Mcc ver: %s\n", VERSION);
	if (argc < 2) {
		show_help();
		return 0;
	}

	parse_args(--argc, ++argv);

	if (is_show_build_version)
		printf("Build version: %s\n", gen_build_ver());
	return 0;
}