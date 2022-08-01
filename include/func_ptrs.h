//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_FUNC_PTRS_H
#define FT_SSL_FUNC_PTRS_H

#include <stddef.h>

typedef int	(*file_handler)(int fd);
typedef int	(*string_handler)(const char *str);

typedef struct s_handler {
	const char * cmd;
	file_handler handle_file;
	string_handler handle_string;
}	t_handler;


int md5sum_string(const char* str);
int md5sum_file(int fd);
int sha256_string(const char* str);
int sha256_file(int fd);

#endif //FT_SSL_FUNC_PTRS_H
