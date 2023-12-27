#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "mcc/mcc_base.h"
#include "mcc/file.h"

__BEGIN_DECLS

int pp_process(File f);
int pp_parse_include(File f);

int pp_process_str(char **ps);
int pp_parse_include_str(char **ps);

__END_DECLS

#endif