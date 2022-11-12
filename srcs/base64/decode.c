//
// Created by pde-bakk on 10/25/22.
//
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "libft.h"
#include "base64/base64.h"

static bool	is_base64_char(char c) {
	return (ft_isalnum(c) || c == '+' || c == '/');
}

char *base64_decode_string(const char *str, size_t datalen, size_t *outlen) {
	*outlen = datalen * 3 / 4;
	uint8_t	*result = calloc(*outlen + 1, sizeof(char));
	size_t	result_i = 0;
	size_t	x = 0;
	uint8_t	char_arr4[4],
			char_arr3[3];

	for (size_t i = 0; i < datalen && (is_base64_char(str[i]) || iswhitespace(str[i]) || str[i] == '='); i++) {
		if (iswhitespace(str[i]) || str[i] == PAD_CHAR) {
			(*outlen)--;
			continue;
		}
		char_arr4[x] = str[i];
		++x;
		if (x == 4) { // we found enough chars
			for (x = 0; x < 4; ++x)
				char_arr4[x] = ft_strchr(BASE64_CHARS, char_arr4[x]) - BASE64_CHARS;
			char_arr3[0] = (char_arr4[0] << 2) + ((char_arr4[1] & 0x30) >> 4);
			char_arr3[1] = ((char_arr4[1] & 0xf) << 4) + ((char_arr4[2] & 0x3c) >> 2);
			char_arr3[2] = ((char_arr4[2] & 0x3) << 6) + char_arr4[3];
			for (x = 0; x < 3; ++x) {
				result[result_i] = char_arr3[x];
				++result_i;
			}
			x = 0;
		}
	}
	if (x) {
		for (size_t j = 0; j < 4; j++)
			char_arr4[j] = ft_strchr(BASE64_CHARS, char_arr4[j]) - BASE64_CHARS;

		char_arr3[0] = (char_arr4[0] << 2) + ((char_arr4[1] & 0x30) >> 4);
		char_arr3[1] = ((char_arr4[1] & 0xF) << 4) + ((char_arr4[2] & 0x3C) >> 2);
		char_arr3[2] = ((char_arr4[2] & 0x3) << 6) + char_arr4[3];

		for (size_t j = 0; j < x - 1; j++) {
			result[result_i] = char_arr3[j];
			++result_i;
		}
	}
	*outlen = result_i;
	return ((char*)result);
}

char *base64_decode_file(const int fd) {
	size_t	outlen;
	struct stat buf;
	char*	file;
	char*	result;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error reading file.\n");
		exit(EXIT_FAILURE);
	}
	result = base64_decode_string(file, buf.st_size, &outlen);
	munmap(file, buf.st_size);
	return (result);
}
