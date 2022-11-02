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
	const int output_fd = get_output_fd();

	if (g_flags & FLAG_ENCODE) {
		return (base64_encode_file(fd, output_fd));
	} else {
		return (base64_decode_file(fd, output_fd));
	}
}

int base64_string(const char* str) {
	const int output_fd = get_output_fd();

	if (g_flags & FLAG_DECODE) {
		return (base64_decode_string(str, output_fd));
	} else {
		return (base64_encode_string(str, output_fd));
	}
}