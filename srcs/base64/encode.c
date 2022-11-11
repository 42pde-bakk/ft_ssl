//
// Created by pde-bakk on 10/25/22.
//
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "libft.h"
#include "base64/base64.h"
#include <sys/stat.h>
#include <sys/mman.h>

uint32_t	combine_three_uint8s(const uint8_t* data, size_t i, size_t datalen) {
	uint32_t grand = ((uint32_t)data[i]) << 16;
	if (i + 1 < datalen) {
		grand |= ((uint32_t)data[i + 1]) << 8;
	}
	if (i + 2 < datalen) {
		grand |= data[i + 2];
	}
	return (grand);
}

char *base64_encode_string(const char *str, size_t datalen, size_t *outlen) {
	const uint8_t*	data = (const uint8_t *)str;
	size_t	pad_count = 3 - datalen % 3;

	*outlen = datalen / 3 * 4;
	if (datalen % 3)
		*outlen += 4;
	if (pad_count == 3)
		pad_count = 0;

	size_t	x = 0;
	uint8_t	*result = calloc(*outlen + 1, sizeof(char));
	for (size_t i = 0; i < datalen; i += 3) {
		uint32_t grand = combine_three_uint8s(data, i, datalen);

		// Now we need to split grand into 4 sextets
		uint8_t sextets[4] = {
				(uint8_t)(grand >> 18) & 63,
				(uint8_t)(grand >> 12) & 63,
				(uint8_t)(grand >> 6) & 63,
				((uint8_t)grand) & 63
		};
		for (size_t tmp = 0; tmp < 4; tmp++) {
			if (tmp > 1 && i + tmp > datalen)
				continue;
			result[x] = BASE64_CHARS[sextets[tmp]];
			++x;
		}
	}
	while (pad_count > 0) {
		result[x] = PAD_CHAR;
		++x;
		--pad_count;
	}
	return ((char *)result);
}

char *base64_encode_file(const int fd) {
	struct stat buf;
	char*	file;
	char*	result;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error rading file.\n");
		exit(EXIT_FAILURE);
	}
	result = base64_encode_string(file, buf.st_size, NULL);
	munmap(file, buf.st_size);
	return (result);
}
