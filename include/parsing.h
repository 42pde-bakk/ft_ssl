//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_PARSING_H
#define FT_SSL_PARSING_H

#include "func_ptrs.h"
#include "vector.h"

unsigned int parse_flags(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector);
t_handler	parse_command(char *cmd);

#endif //FT_SSL_PARSING_H
