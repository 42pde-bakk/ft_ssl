//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_FUNC_PTRS_H
#define FT_SSL_FUNC_PTRS_H

typedef int	(*file_handler)(int fd, char **digest);
typedef int	(*string_handler)(const char *str, char **digest);

typedef struct s_handler {
	const char * cmd;
	file_handler handle_file;
	string_handler handle_string;
//	int (*handle_file)(int fd, unsigned int flags);
//	int (*handle_string)(const char *str, unsigned int flags);
}	t_handler;


int md5sum_string(const char* str, char **digest);
int md5sum_file(int fd, char **digest);
int sha256_string(const char* str, char **digest);
int sha256_file(int fd, char **digest);

#endif //FT_SSL_FUNC_PTRS_H
