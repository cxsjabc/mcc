#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_show_build_version, is_show_help, is_show_version = 1;

void parse_args(int argc, char *argv[])
{
    while (argc--) {
        if (strcmp(*argv, "-b") == 0 || strcmp(*argv, "--build") == 0) {
            is_show_build_version = 1;
        } else if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0) {
            is_show_help = 1;
        } else if (strcmp(*argv, "-v") == 0 || strcmp(*argv, "--version") == 0) {
            is_show_version = 1;
        }
    }
}