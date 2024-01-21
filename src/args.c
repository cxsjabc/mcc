#define NO_DEBUG 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/compile_log.h"
#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/string.h"

__BEGIN_DECLS

static int args_post_process(MccState ms);
static void args_dump_info(MccState ms);

struct args_option GloArgOption;

const char *get_output_file_name()
{
	return GloArgOption.out_file.str;
}

int parse_args(int argc, char *argv[], MccState ms)
{
	int r = ERR_NONE;

	if (ms == NULL)
		ms = &MS;

	while (argc--) {
		debug("arg: %s\n", *argv ? *argv : "null");
		if (strcmp(*argv, "-b") == 0 || strcmp(*argv, "--build") == 0) {
			ARG_VAR_BUILD_VER0 = 1;
		} else if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0) {
			ARG_VAR_HELP0 = 1;
		} else if (strcmp(*argv, "-v") == 0 || strcmp(*argv, "--version") == 0) {
			ARG_VAR_VERSION0 = 1;
		} else if (strcmp(*argv, "-E") == 0) { // preprocess only
			ARG_VAR_PREPROCESS_ONLY0 = 1;
		} else if (strcmp(*argv, "-c") == 0) { // compile only
			ARG_VAR_COMPILE_ONLY0 = 1;
		} else if (strcmp(*argv, "-S") == 0) { // assemble
			ARG_VAR_ASSEMBLE_ONLY0 = 1;
		} else if (strcmp(*argv, "-o") == 0) { // output file
			LHD;
			ARG_VAR_HAS_OUTPUT_FILE0 = 1;
			++argv, --argc;
			if (*argv) {
				r = cstr_init(&GloArgOption.out_file, *argv, strlen(*argv));
				if (r != ERR_NONE)
					ERR_RETURN(r);
				debug("Output file name: %s\n", get_output_file_name());
				ARG_VAR_HAS_OUTPUT_FILE0 = 1;
			} else {
				cerror("No output file specified\n");
				return ERR_INVALID_PARAM;
			}
		} else if (strncmp(*argv, "-I", 2) == 0 || strncmp(*argv, "-L", 2) == 0) {  // include pathes or library pathes
			const char *p = *argv;
			int is_include_path = p[1] == 'I';
			int is_lib_path = p[1] == 'L';
			int file_type = FILE_TYPE_UNK;
			
			if (is_include_path)
				file_type = FILE_TYPE_HEADER_PATH;
			if (is_lib_path)
				file_type = FILE_TYPE_LIB_PATH;
			LHD;
			debug("file_type: %s\n", FileTypeNames[FILE_TYPE_IDX(file_type)]);
			if (p[2] == '\0') {
				++argv, --argc;
				if (argv && (*argv)[0] != '-') {
					r = mcc_state_add_files(ms, *argv, (FileType)file_type);
					if (r < OK) {
						error("Failed to parse file: %s, ret(%d)\n", *argv, r);
						ERR_RETURN(r);
					}
					debug("Adding file: %s(%s)\n", *argv, FileTypeNames[FILE_TYPE_IDX(r)]);
				} else
					error("Invalid include or lib path: %s\n", *argv);
			} else {
				const char *path = &p[2];
				r = mcc_state_add_files(ms, path, (FileType)file_type);
				if (r < OK) {
					error("Failed to parse file: %s\n", path);
					ERR_RETURN(r);
				}
				debug("Adding file: %s(%s)\n", path, FileTypeNames[FILE_TYPE_IDX(r)]);
			}
		} else {
			// not option, means source/header files or objects/target files
			if (strcmp(*argv, "-") != 0) {
				r = mcc_state_add_files(ms, *argv, FILE_TYPE_UNK);
				if (r < OK) {
					error("Failed to parse file: %s\n", *argv);
					ERR_RETURN(r);
				}
				debug("Adding file: %s(%s)\n", *argv, FileTypeNames[FILE_TYPE_IDX(r)]);
			} else
				error("Unexpected option or parameter: %s\n", *argv);
		}
		++argv;
	}

	if (r >= OK)
		r = args_post_process(ms);

	return r;
}

static int args_post_process(MccState ms)
{
	DynArray src_files = ms->src_files;
	int srcs_size;
	int r = OK;

	srcs_size = dynamic_array_size(src_files);
	
	if (ARG_VAR_HAS_OUTPUT_FILE0) {
		if (srcs_size > 1) {
			if (ARG_VAR_PREPROCESS_ONLY0 || ARG_VAR_COMPILE_ONLY0 || ARG_VAR_ASSEMBLE_ONLY0) {
				cerror("-o can't be used with -E -S -c with multiple files\n");
				r = ERR_INVALID_PARAM;
			}
		}
	}

	args_dump_info(ms);

	return r;
}

static void args_dump_info(MccState ms)
{
	if (ARG_VAR_PREPROCESS_ONLY0)
		debug_nofl("only preprocess ");
	if (ARG_VAR_COMPILE_ONLY0)
		debug_nofl("only compile ");
	if (ARG_VAR_ASSEMBLE_ONLY0)
		debug_nofl("only assemble ");
	if (ARG_VAR_HAS_OUTPUT_FILE0)
		always("output file: \"%s\"\n", get_output_file_name());

	vec_dump1(ms->include_paths, "Include paths: ", "Include paths ends\n");
	vec_dump1(ms->src_files, "Src files: ", "Src files ends\n");
	vec_dump1(ms->lib_paths, "Lib paths: ", "Lib paths ends\n");
	vec_dump1(ms->obj_files, "Obj files: ", "Obj files ends\n");
}

void args_init()
{
	args_clean_state();
}

void args_clean_state()
{
	memset(&GloArgOption, 0, sizeof(GloArgOption));
	// default enable show mcc version
	ARG_VAR_VERSION0 = 1;
}

__END_DECLS