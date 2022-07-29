//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_FUNC_PTRS_H
#define FT_SSL_FUNC_PTRS_H

#include <stddef.h>

typedef int	(*file_handler)(int fd, char **digest);
typedef int	(*string_handler)(const char *str, char **digest);

typedef struct s_handler {
	const char * cmd;
	file_handler handle_file;
	string_handler handle_string;
	size_t	digest_length;
}	t_handler;


int md5sum_string(const char* str, char **digest);
int md5sum_file(int fd, char **digest);
int sha256_string(const char* str, char **save_digest);
int sha256_file(int fd, char **digest);

#endif //FT_SSL_FUNC_PTRS_H
