//
// Created by peer on 9-11-22.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "des/flags.h"
#include "base64/base64.h"
#include "libft.h"
#include "des/des.h"

static uint64_t REV64(uint64_t x) {
	int				i = 0;
	uint64_t		y = 0;
	unsigned char	*ptr_x,
			*ptr_y;
	size_t			size = sizeof(x);

	ptr_x = (unsigned char *)&x;
	ptr_y = (unsigned char *)&y;
	while (--size)
		ptr_y[i++] = ptr_x[size];
	ptr_y[i++] = ptr_x[size];
	return (y);
}

void	output_chunk(const int fd, uint64_t chunk) {
	char	charr[9];

	chunk = REV64(chunk);
	ft_bzero(charr, sizeof(charr));
	ft_memcpy(charr, &chunk, CHUNK_SIZE_IN_BYTES);

	if (g_des_flags & FLAG_BASE64) {
		base64_encode_string(charr, fd);
	} else {
		ssize_t ret = write(fd, charr, CHUNK_SIZE_IN_BYTES);
		if (ret < 0) {
			fprintf(stderr, "Write failed.\n");
			exit(EXIT_FAILURE);
		}
	}
}
