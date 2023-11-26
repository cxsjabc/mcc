//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/ver.h"
#include "mcc/args.h"
#include "mcc/help.h"
#include "mcc/log.h"

int main(int argc, char *argv[])
{
	always("Mcc ver: %s\n", VERSION);

	if (argc < 2) {
		show_help();
		return 0;
	}

	parse_args(--argc, ++argv);

	if (is_show_build_version)
		always("Build version: %s\n", gen_build_ver());
	if (is_show_help)
		show_help();
	return 0;
}