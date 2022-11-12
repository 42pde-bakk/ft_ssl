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

static int des_ofb_handler(const char* str, size_t length) {
	const uint64_t	key = get_key();
	uint64_t		chunk,
					result,
					iv;
	char*			base = NULL;
	char*			padded_str = NULL;



	if (!(g_des_flags & FLAG_INITVECTOR) || !g_initialization_vector) {
		dprintf(STDERR_FILENO, "For ofb mode please provide the initialization vector with the -v option.\n");
	}
	iv = create_64bit_chunk_from_hexstr(g_initialization_vector);
	if (g_des_flags & FLAG_SHOW_KEY)
		dprintf(STDERR_FILENO, "iv= %016lX\n", iv);

	if (!(g_des_flags & FLAG_NO_PADDING) && g_des_flags & FLAG_ENCODE) {
		const uint8_t pad_amount = 8 - (length % 8);
		padded_str = ft_calloc(length + pad_amount, sizeof(char));
		ft_strlcpy(padded_str, str, length + 1);

		for (uint8_t i = 0; i < pad_amount; i++) {
			padded_str[length + i] = (char)pad_amount;
		}
		str = padded_str;
		length += pad_amount;
	}

	if (g_des_flags & FLAG_DECODE) {
		if (g_des_flags & FLAG_BASE64) {
			size_t newdatalen;
			base = base64_decode_string(str, length, &newdatalen);
			str = base;
			length = newdatalen;
		}
		unsigned int tmpflags = g_des_flags;
		g_des_flags = FLAG_ENCODE;
		for (size_t i = 0; i < length; i += CHUNK_SIZE_IN_BYTES) {
			chunk = REV64(*(uint64_t *)(str + i));

			iv = apply_des(iv, key);
			result = iv ^ chunk;
			add_chunk_to_buffer(result, true);
		}
		g_des_flags = tmpflags;

	} else { // FLAG_ENCODE
		for (size_t i = 0; i < length; i += CHUNK_SIZE_IN_BYTES) {
			chunk = create_64bit_chunk_from_str(str + i);

			iv = apply_des(iv, key);
			result = iv ^ chunk;
			add_chunk_to_buffer(result, true);
		}
	}

	if (!(g_des_flags & FLAG_NO_PADDING)) {
		free(padded_str);
	}

	clear_buffer(g_outfd, false);
	free(base);
	return (EXIT_SUCCESS);
}

int des_ofb_fd(const int fd) {
	int return_status;
	struct stat buf;
	char* file;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error reading file.\n");
		return (EXIT_FAILURE);
	}
	return_status = des_ofb_handler(file, buf.st_size);
	dprintf(g_outfd, "\n");
	munmap(file, buf.st_size);
	return (return_status);
}

int des_ofb_string(const char* str) {
	return (des_ofb_handler(str, ft_strlen(str)));
}
