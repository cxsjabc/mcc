//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/args.h"
#include "mcc/error.h"
#include "mcc/help.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/ver.h"

int main(int argc, char *argv[])
{
	int r;

	always("Mcc ver: %s\n", VERSION);

	if (argc < 2) {
		show_help();
		return 0;
	}

	r = parse_args(--argc, ++argv);
	if (r != ERR_NONE)
        return r;

	if (is_show_build_version)
		always("Build version: %s\n", gen_build_ver());
	if (is_show_help)
		show_help();

	setup_mcc_state();

	return 0;
}