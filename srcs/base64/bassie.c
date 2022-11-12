//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "base64/flags.h"
#include "base64/base64.h"
#include "libft.h"

int base64_fd(const int fd) {
	char*	result;

	if (g_base64_flags & FLAG_ENCODE) {
		result = base64_encode_file(fd);
	} else {
		result = base64_decode_file(fd);
	}
	dprintf(g_outfd, "%s\n", result);
	free(result);
	return (EXIT_SUCCESS);
}

int base64_string(const char* str) {
	char*	result;
	size_t	str_len = ft_strlen(str);
	size_t	output_len;

	if (g_base64_flags & FLAG_DECODE) {
		result = base64_decode_string(str, str_len, &output_len);
	} else {
		result = base64_encode_string(str, str_len, &output_len);
	}
	dprintf(g_outfd, "%s", result);
	free(result);
	return (EXIT_SUCCESS);
}
