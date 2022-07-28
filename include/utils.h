//
// Created by Peer de Bakker on 7/27/22.
//

#ifndef FT_SSL_UTILS_H
#define FT_SSL_UTILS_H

char	*string_toupper(char *str);
char	*get_escaped_string(char *str);
char	*get_program_name(char *argv0);

void	print_usage(const char *prog_name);
void	print_error(const char* prog_name, char* invalid_command);

#endif //FT_SSL_UTILS_H
