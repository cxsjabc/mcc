//#define NO_DEBUG 1

#include <stdio.h>

#include "mcc/args.h"
#include "mcc/compile.h"
#include "mcc/env.h"
#include "mcc/error.h"
#include "mcc/help.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/prepare.h"
#include "mcc/ver.h"

__BEGIN_DECLS

int main(int argc, char *argv[])
{
	int r;

	always("Mcc ver: %s\n", VERSION);

	if (argc < 2) {
		show_help();
		OK_RETURN(0);
	}

	prepare();

	// now, use global mcc state
	r = parse_args(--argc, ++argv, &MS);
	if (r != ERR_NONE)
		goto error_parse_args;
	// dump_mcc_state(&MS);

	if (ARG_VAR_BUILD_VER0)
		always("Build version: %s\n", gen_build_ver());
	if (ARG_VAR_HELP0)
		show_help();

	r = compile(&MS);

error_parse_args:
	destroy_mcc_state(&MS);
	uninit_global_mem_buf();
	return r;
}

__END_DECLS