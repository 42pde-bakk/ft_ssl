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
#include "vector.h"

static t_uint64vector*	chunk_vector = NULL;

uint64_t REV64(uint64_t x) {
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

void	create_str_from_64bit_chunk_and_output(uint64_t chunk, const int fd) {
	char	arr[9];

	ft_bzero(arr, sizeof(arr));
	for (size_t i = 0; i < 8; i++) {
		arr[i] = (char)chunk;
		chunk >>= 8;
	}
	if (write(fd, arr, CHUNK_SIZE_IN_BYTES) == -1) {
		fprintf(stderr, "Write failed.\n");
		exit(EXIT_FAILURE);
	}
}

void add_chunk_to_buffer(uint64_t chunk, bool should_reverse) {
	if (chunk_vector == NULL) {
		chunk_vector = uint64vector_init(128);
		if (chunk_vector == NULL)
			exit(EXIT_FAILURE);
	}

	if (should_reverse)
		chunk = REV64(chunk);
	uint64vector_pushback(chunk_vector, chunk);
}

void	clear_buffer(const int fd) {
	if (g_des_flags & FLAG_BASE64 && g_des_flags & FLAG_ENCODE) {
//		for (size_t i = 0; i < chunk_vector->size; i++) {
//			dprintf(STDERR_FILENO, "E2: chunk %zu = %016lX (%s)\n", i, chunk_vector->arr[i], (char *)&chunk_vector->arr[i]);
//		}
		char* result = base64_encode_string((char *)chunk_vector->arr, chunk_vector->size * CHUNK_SIZE_IN_BYTES);
		dprintf(fd, "%s", result);
		free(result);
	} else {
		for (size_t i = 0; i < chunk_vector->size; i++) {
//			dprintf(STDERR_FILENO, "write(%d, %016lX, %d);\n", fd, chunk_vector->arr[i], CHUNK_SIZE_IN_BYTES);
			create_str_from_64bit_chunk_and_output(chunk_vector->arr[i], fd);
		}
	}
	uint64vector_destroy(chunk_vector);
	chunk_vector = NULL;
}
