#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc/error.h"
#include "mcc/file.h"
#include "mcc/log.h"
#include "mcc/string.h"

int is_show_version = 1;
int is_show_build_version, is_show_help;
int is_preprocess;
int is_output_file;

static Cstr out_file;

const char *get_output_file_name()
{
    return out_file.str;
}

int parse_args(int argc, char *argv[])
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
                    return r;
                debug("Preprocessed file output name: %s\n", get_output_file_name());
            }
        } else if (strcmp(*argv, "-I") == 0) {  // include pathes
            ++argv, --argc;
            while (*argv && (*argv)[0] != '-') {
                r = mcc_state_add_files(argv, FILE_TYPE_H);
                if (r != ERR_NONE) {
                    error("Failed to parse file: %s\n", argv);
                    return r;
                }
            }
            --argv, ++argc;
        } else {
            // not option, means source/header files or objects/target files
            if (strcmp(*argv, "-") != 0) {
                r = mcc_state_add_files(argv);
                if (r != ERR_NONE) {
                    error("Failed to parse file: %s\n", argv);
                    return r;
                }
            }
        }
        ++argv;
    }

    return r;
}