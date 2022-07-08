//
// Created by Peer de Bakker on 7/4/22.
//

#ifndef FT_SSL_FUNC_PTRS_H
#define FT_SSL_FUNC_PTRS_H

typedef int	(*cmd_func_t)(unsigned int flags, const char *filename);

int	md5sum_file(unsigned int flags, const char *filename);
int	sha256(unsigned int flags, const char *filename);

#endif //FT_SSL_FUNC_PTRS_H
