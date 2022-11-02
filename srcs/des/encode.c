//
// Created by pde-bakk on 11/2/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include <string.h>
#include "des/des.h"
#include "des/flags.h"

int	des_encode_string(const char* str) {
	const size_t datalen = ft_strlen(str);

	for (size_t i = 0; i < datalen; i += CHUNK_SIZE) {
		char* sub = ft_substr(str, i, CHUNK_SIZE);
	}
	return (0);
}

int des_encode_fd(const int fd) {
	(void)fd;
	return (0);
}