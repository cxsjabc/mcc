#ifndef ARGS_H
#define ARGS_H

#include "mcc/mcc_base.h"
#include "mcc/mcc_state.h"
#include "mcc/string.h"

__BEGIN_DECLS

extern struct args_option GloArgOption;

typedef struct args_option
{
	unsigned int is_show_version : 1;
	unsigned int is_show_build_version : 1;
	unsigned int is_show_help : 1;

	unsigned int is_preprocess_only : 1;
	unsigned int is_compile_only : 1;
	unsigned int is_assemble_only : 1;
	unsigned int has_output_file : 1;

	unsigned int is_verbosed : 1;

	struct cstr out_file;
} *Args_option;

#define ARG_VAR_VERSION(opt) ((opt).is_show_version)
#define ARG_VAR_BUILD_VER(opt) ((opt).is_show_build_version)
#define ARG_VAR_HELP(opt) ((opt).is_show_help)

#define ARG_VAR_PREPROCESS_ONLY(opt) ((opt).is_preprocess_only)
#define ARG_VAR_COMPILE_ONLY(opt) ((opt).is_compile_only)
#define ARG_VAR_ASSEMBLE_ONLY(opt) ((opt).is_assemble_only)
#define ARG_VAR_HAS_OUTPUT_FILE(opt) ((opt).has_output_file)
#define ARG_VAR_VERBOSED(opt) ((opt).is_verbosed)

// Use default global argument option variable
#define ARG_VAR_VERSION0 ARG_VAR_VERSION(GloArgOption)
#define ARG_VAR_BUILD_VER0 ARG_VAR_BUILD_VER(GloArgOption)
#define ARG_VAR_HELP0 ARG_VAR_HELP(GloArgOption)

#define ARG_VAR_PREPROCESS_ONLY0 ARG_VAR_PREPROCESS_ONLY(GloArgOption)
#define ARG_VAR_COMPILE_ONLY0 ARG_VAR_COMPILE_ONLY(GloArgOption)
#define ARG_VAR_ASSEMBLE_ONLY0 ARG_VAR_ASSEMBLE_ONLY(GloArgOption)
#define ARG_VAR_HAS_OUTPUT_FILE0 ARG_VAR_HAS_OUTPUT_FILE(GloArgOption)
#define ARG_VAR_VERBOSED0 ARG_VAR_VERBOSED(GloArgOption)

int parse_args(int argc, char *argv[], MccState ms);

void args_clean_state();

__END_DECLS

#endif