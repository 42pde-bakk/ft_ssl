//
// Created by pde-bakk on 10/25/22.
//
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "libft.h"
#include "base64/base64.h"

int	base64_string(const char* str) {
	const uint8_t*	data = (const uint8_t *)str;
	size_t	datalen = ft_strlen(str);
	size_t	x = 0;
	uint8_t*	result = calloc(datalen / 4 * 3 + 1, sizeof(char));
	fprintf(stderr, "datalen = %zu, str=%s\n", datalen, str);

	for (size_t i = 0; i < datalen; i += 3) {
		fprintf(stderr, "i = %zu\n", i);
		fprintf(stderr, "octets: [%d, %d, %d]\n", data[i], data[i+1], data[i+2]);
		uint32_t grand = ((uint32_t)data[i]) << 16;
		if (i + 1 < datalen) {
			grand |= ((uint32_t)data[i + 1]) << 8;
		}
		if (i + 2 < datalen) {
			grand |= data[i + 2];
		}
//		uint32_t grand = (data[i] << 16) + (data[i + 1] << 8) + (data[i + 2]);
		// Now we need to split grand into 4 sextets
		uint8_t sextets[4] = {
				(uint8_t)(grand >> 18) & 63,
				(uint8_t)(grand >> 12) & 63,
				(uint8_t)(grand >> 6) & 63,
				((uint8_t)grand) & 63
		};
		for (size_t tmp = 0; tmp < 4; tmp++) {
			result[x] = BASE64_CHARS[sextets[tmp]];
			++x;
		}
	}
	printf("final string = %s, x=%zu\n", result, x);

	return (0);
}

int	base64_file(const int fd) {
	(void)fd;
	return (0);
}