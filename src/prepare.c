#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mcc/env.h"
#include "mcc/id.h"
#include "mcc/lex.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/mem_buf.h"
#include "mcc/prepare.h"
#include "mcc/symbol.h"

__BEGIN_DECLS

void prepare()
{
	check_build_environment();
	check_running_environment();

	// prepare structure and mem buffer for compiling
	init_from_exist_mcc_state(&MS, 0);
	setup_global_mem_buf();
	init_char_type_table();
	lex_init();
	sym_global_init();
}

__END_DECLS