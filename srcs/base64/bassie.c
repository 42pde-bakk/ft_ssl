//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include "ft_printf.h"
#include <stdlib.h>
#include "base64/flags.h"
#include "base64/base64.h"
#include "libft.h"

int base64_fd(const int fd) {
	char*	result;

	if (g_base64_flags & FLAG_ENCRYPT) {
		result = base64_encode_file(fd);
	} else {
		result = base64_decode_file(fd);
	}
	ft_dprintf(g_outfd, "%s", result);
	if (g_base64_flags & FLAG_ENCRYPT) {
		ft_dprintf(g_outfd, "\n");
	}
	free(result);
	return (EXIT_SUCCESS);
}

int base64_string(const char *str, size_t length) {
	char*	result;
	size_t	output_len;

	if (g_base64_flags & FLAG_DECRYPT) {
		result = base64_decode_string(str, length, &output_len);
	} else {
		result = base64_encode_string(str, length, &output_len);
	}
	ft_dprintf(g_outfd, "%s", result);
	if (g_base64_flags & FLAG_ENCRYPT) {
		ft_dprintf(g_outfd, "\n");
	}
	free(result);
	return (EXIT_SUCCESS);
}
