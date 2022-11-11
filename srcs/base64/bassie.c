//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "base64/flags.h"
#include "base64/base64.h"
#include "libft.h"

int	get_output_fd() {
	int fd;

	if (g_output == NULL)
		return (STDOUT_FILENO);
	fd = open(g_output, O_WRONLY);
	if (fd == -1) {
		fprintf(stderr, "Error, bad outputfile given\n");
		exit(1);
	}
	return (fd);
}

int base64_fd(const int fd) {
	char*	result;

	if (g_base64_flags & FLAG_ENCODE) {
		result = base64_encode_file(fd);
	} else {
		result = base64_decode_file(fd);
	}
	dprintf(STDOUT_FILENO, "%s", result);
	free(result);
	return (EXIT_SUCCESS);
}

int base64_string(const char* str) {
	char*	result;
	size_t	str_len = ft_strlen(str);

	if (g_base64_flags & FLAG_DECODE) {
		result = base64_decode_string(str, str_len, NULL);
	} else {
		result = base64_encode_string(str, str_len, NULL);
	}
	dprintf(STDOUT_FILENO, "%s", result);
	free(result);
	return (EXIT_SUCCESS);
}
