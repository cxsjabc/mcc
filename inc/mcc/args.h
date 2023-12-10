#ifndef ARGS_H
#define ARGS_H

#include "mcc/features.h"
#include "mcc/mcc_state.h"

__BEGIN_DECLS

extern int is_show_build_version, is_show_help, is_show_version;

int parse_args(int argc, char *argv[], MccState ms);

__END_DECLS

#endif