//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include "des/flags.h"
#include "des/des.h"
#include "libft.h"
#include "base64/base64.h"

int des_cbc_fd(const int fd) {
	const uint64_t	key = get_key();
	struct stat buf;
	char* file;
	uint64_t	result = 0,
				chunk;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error rading file.\n");
		return (EXIT_FAILURE);
	}
	if (g_des_flags & FLAG_INITVECTOR && g_initialization_vector)
		result = create_64bit_chunk_from_hexstr(g_initialization_vector);

	if (g_des_flags & FLAG_SHOW_KEY)
		printf("iv= %016lX\n", result);

	if (g_des_flags & FLAG_BASE64 && g_des_flags & FLAG_DECODE) {
		char* base = base64_decode_string(file, 0);
		for (size_t i = 0; i < ft_strlen(base); i += CHUNK_SIZE_IN_BYTES) {
			chunk = create_64bit_chunk_from_str(base + i);
			result = apply_des(chunk ^ result, key);
			add_chunk_to_buffer(result, 0); // TODO: fix output file
		}
		free(base);
	} else {
		for (long int i = 0; i < buf.st_size; i += CHUNK_SIZE_IN_BYTES) {
			chunk = create_64bit_chunk_from_str(file + i);
			result = apply_des(chunk ^ result, key);
			add_chunk_to_buffer(result, 0);
		}
	}
	munmap(file, buf.st_size);
	clear_buffer(1);
	return (EXIT_SUCCESS);
}

int des_cbc_string(const char* str) {
	size_t datalen = ft_strlen(str);
	const uint64_t	key = get_key();
	uint64_t	result = 0,
				chunk;
	char*			base = NULL;

	if (g_des_flags & FLAG_INITVECTOR && g_initialization_vector)
		result = create_64bit_chunk_from_hexstr(g_initialization_vector);
	if (g_des_flags & FLAG_SHOW_KEY)
		dprintf(STDERR_FILENO, "iv= %016lX\n", result);

	if (g_des_flags & FLAG_DECODE) {
		if (g_des_flags & FLAG_BASE64) {
			base = base64_decode_string(str, datalen);
//			dprintf(STDERR_FILENO, "got string %s which amounts to %s in base64, strlen = %zu\n", str, base, ft_strlen(base));
			str = base;
			datalen = ft_strlen(str);
		}
		for (size_t i = 0; i < datalen; i += CHUNK_SIZE_IN_BYTES) {
			chunk = REV64(*(uint64_t *)(str + i));

			result = apply_des(chunk, key) ^ result;
//			dprintf(STDERR_FILENO, "D2: chunk = %016lX, result = %016lX\n", chunk, result);
			add_chunk_to_buffer(result, true);
		}

	} else { // FLAG_ENCODE
		for (size_t i = 0; i < datalen; i += CHUNK_SIZE_IN_BYTES) {
			chunk = create_64bit_chunk_from_str(str + i);

			result = apply_des(chunk ^ result, key);
//			dprintf(STDERR_FILENO, "E1: chunk = %016lX, result = %016lX\n", chunk, result);
			add_chunk_to_buffer(result, true);
		}
	}

	clear_buffer(g_outfd);
	return (EXIT_SUCCESS);
}
