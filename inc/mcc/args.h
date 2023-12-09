#ifndef ARGS_H
#define ARGS_H

#include "mcc/mcc_state.h"

extern int is_show_build_version, is_show_help, is_show_version;

int parse_args(int argc, char *argv[], MccState ms);

#endif