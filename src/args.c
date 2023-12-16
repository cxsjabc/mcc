#define NO_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/mcc_state.h"
#include "mcc/string.h"

__BEGIN_DECLS

int is_show_version = 1;
int is_show_build_version, is_show_help;
int is_preprocess;
int is_output_file;

static Cstr out_file;

const char *get_output_file_name()
{
	return out_file.str;
}

int parse_args(int argc, char *argv[], MccState ms)
{
	int r = ERR_NONE;

	while (argc--) {
		if (strcmp(*argv, "-b") == 0 || strcmp(*argv, "--build") == 0) {
			is_show_build_version = 1;
		} else if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0) {
			is_show_help = 1;
		} else if (strcmp(*argv, "-v") == 0 || strcmp(*argv, "--version") == 0) {
			is_show_version = 1;
		} else if (strcmp(*argv, "-E") == 0) {
			is_preprocess = 1;
			++argv, --argc;
			if (*argv) {
				r = init_cstr(&out_file, *argv, strlen(*argv));
				if (r != ERR_NONE)
					ERR_RETURN(r);
				debug("Preprocessed file output name: %s\n", get_output_file_name());
			}
		} else if (strncmp(*argv, "-I", 2) == 0 || strncmp(*argv, "-L", 2) == 0) {  // include pathes or library pathes
			const char *p = *argv;
			int is_include_path = p[1] == 'I';
			int is_lib_path = p[1] == 'L';
			int file_type = FILE_TYPE_UNKNOWN;
			
			if (is_include_path)
				file_type = FILE_TYPE_HEADER_PATH;
			if (is_lib_path)
				file_type = FILE_TYPE_LIB_PATH;
			debug("file_type: %s\n", FileTypeNames[file_type]);
			if (p[2] == '\0') {
				++argv, --argc;
				if (argv && (*argv)[0] != '-') {
					r = mcc_state_add_files(ms, *argv, (FileType)file_type);
					if (r != ERR_NONE) {
						error("Failed to parse file: %s, ret(%d)\n", *argv, r);
						ERR_RETURN(r);
					}
					debug("Adding file: %s(%s)\n", *argv, FileTypeNames[file_type]);
				} else
					error("Invalid include path: %s\n", *argv);
			} else {
				const char *path = &p[2];
				r = mcc_state_add_files(ms, path, (FileType)file_type);
				if (r != ERR_NONE) {
					error("Failed to parse file: %s\n", path);
					ERR_RETURN(r);
				}
				debug("Adding file: %s(%s)\n", path, FileTypeNames[file_type]);
			}
		} else {
			// not option, means source/header files or objects/target files
			if (strcmp(*argv, "-") != 0) {
				r = mcc_state_add_files(ms, *argv, FILE_TYPE_C);
				if (r != ERR_NONE) {
					error("Failed to parse file: %s\n", *argv);
					ERR_RETURN(r);
				}
				debug("Adding file: %s(%s)\n", *argv, FileTypeNames[FILE_TYPE_C]);
			} else
				error("Unexpected option or parameter: %s\n", *argv);
		}
		++argv;
	}

	return r;
}

__END_DECLS