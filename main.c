//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/args.h"
#include "mcc/env.h"
#include "mcc/error.h"
#include "mcc/help.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/ver.h"

int main(int argc, char *argv[])
{
	int r;

	always("Mcc ver: %s\n", VERSION);

	check_build_envionment();

	if (argc < 2) {
		show_help();
		OK_RETURN(0);
	}

	setup_mcc_state(&MS);

	// now, use global mcc state
	r = parse_args(--argc, ++argv, &MS);
	if (r != ERR_NONE)
        ERR_RETURN(r);
	// dump_mcc_state(&MS);

	if (is_show_build_version)
		always("Build version: %s\n", gen_build_ver());
	if (is_show_help)
		show_help();

	return 0;
}