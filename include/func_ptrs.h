//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_FUNC_PTRS_H
#define FT_SSL_FUNC_PTRS_H

#include <stddef.h>
#include "vector.h"

typedef int	(*func_parser)(char** argv);
typedef int	(*file_handler)(int fd);
typedef int	(*string_handler)(const char *str, size_t length);
typedef unsigned int (*flag_parser)(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector);

typedef struct s_handler {
	const char * cmd;
	func_parser	handler;

	file_handler	handle_file;
	string_handler	handle_string;
	flag_parser		handle_flags;
	bool			print_filename;
}	t_handler;

int md5sum_string(const char *str, size_t length);
int md5sum_file(int fd);

int sha256_string(const char *str, size_t length);
int sha256_file(int fd);

int sha224_string(const char *str, size_t length);
int sha224_file(int fd);

int base64_string(const char *str, size_t length);
int base64_fd(int fd);

int des_ecb_string(const char *str, size_t length);
int des_ecb_fd(int fd);

int des_cbc_string(const char *str, size_t length);
int des_cbc_fd(int fd);

int des_ctr_string(const char *str, size_t length);
int des_ctr_fd(int fd);

int des_ofb_string(const char *str, size_t length);
int des_ofb_fd(int fd);

int des_cfb_fd(int fd);
int des_cfb_string(const char *str, size_t length);

int des_pcbc_fd(int fd);
int des_pcbc_string(const char *str, size_t length);

#endif //FT_SSL_FUNC_PTRS_H
