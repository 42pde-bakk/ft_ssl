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
#include <unistd.h>

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

static void create_str_from_64bit_chunk_and_output(uint64_t chunk, const int fd, const size_t write_len) {
	char	arr[9];

	ft_bzero(arr, sizeof(arr));

	for (size_t i = 0; i < 8; i++) {
		arr[i] = (char)chunk;
		chunk >>= 8;
	}
	if (write(fd, arr, write_len) == -1) {
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

uint8_t	remove_padding() {
	uint64_t	*last_chunk = &chunk_vector->arr[chunk_vector->size - 1];
	uint8_t		pad_amount = *last_chunk & 0x000000FF;

	if (pad_amount == 0 || pad_amount > 8) {
		dprintf(STDERR_FILENO, "Warning, invalid padding scheme, found %#hhx.\n", pad_amount);
		return (0);
	}

	for (size_t i = 0; i < pad_amount; i++) {
		uint8_t c = (*last_chunk >> (8 * i)) & 0xFF;
		if (c != pad_amount) {
			dprintf(STDERR_FILENO, "Warning, invalid padding scheme, expected %#hhx, but found %#hhx.\n", pad_amount, c);
			return (0);
		}
	}

	if (pad_amount == 0x8) {
		uint64vector_delete_bypos(chunk_vector, chunk_vector->size - 1);
		return (0);
	}
//	*last_chunk = *last_chunk >> (8 * pad_amount);
//	*last_chunk = *last_chunk << (8 * pad_amount);
//	dprintf(2, "now the chunk looks like %016lX\n", *last_chunk);
	return (pad_amount);
}

void clear_buffer(const int fd, const bool reverse_decode) {
	size_t padding_removed = 0;

	if (!(g_des_flags & FLAG_NO_PADDING) && g_des_flags & FLAG_DECRYPT) {
		padding_removed = remove_padding();
	}
	if (g_des_flags & FLAG_BASE64 && g_des_flags & FLAG_ENCRYPT) {
		size_t newdatalen;
		char* result = base64_encode_string((char *) chunk_vector->arr, chunk_vector->size * CHUNK_SIZE_IN_BYTES, &newdatalen);
		dprintf(fd, "%s", result);
		free(result);
	} else {
		for (size_t i = 0; i < chunk_vector->size; i++) {
			size_t write_length = CHUNK_SIZE_IN_BYTES;
			if (i == chunk_vector->size - 1)
				write_length -= padding_removed;
			if (g_des_flags & FLAG_DECRYPT && reverse_decode)
				chunk_vector->arr[i] = REV64(chunk_vector->arr[i]);
			create_str_from_64bit_chunk_and_output(chunk_vector->arr[i], fd, write_length);
		}
	}
	uint64vector_destroy(chunk_vector);
	chunk_vector = NULL;
}
