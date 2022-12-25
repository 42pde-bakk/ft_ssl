//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_PARSING_H
#define FT_SSL_PARSING_H

#include "func_ptrs.h"
#include "vector.h"

unsigned int parse_flags_base64(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector);

unsigned int parse_flags_des(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector);

unsigned int parse_flags_md5_sha(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector);

t_handler parse_command(const char* const cmd);

#endif //FT_SSL_PARSING_H
