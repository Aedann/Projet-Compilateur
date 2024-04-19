
#ifndef _COMMON_H_
#define _COMMON_H_

#include "defs.h"

#define MINICC_HELP "MiniCC HELP\n\nSYNOPSIS\nminicc [-b=banner]\nminicc input_file [-o output_file] [-t trace_level] [-r number_of_registers] \
[-s=stop_after_syntax] [-v=stop_after_verif] [-h=display_help_and_stop_parsing]\n\n\
OPTIONS\ntrace_level must be between 0 and 5\nnumber_of_registers must be between 4 and 8\n"

void parse_args(int argc, char ** argv);
void free_nodes(node_t n);
char * strdupl(char * s);
void dump_tree(node_t prog_root, const char * filename);
const char * node_type2string(node_type t);
const char * node_nature2string(node_nature t);
const char * node_nature2symb(node_nature t);



#endif

