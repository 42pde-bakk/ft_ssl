//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include "ft_printf.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include "des/flags.h"
#include "des/des.h"
#include "libft.h"
#include "base64/base64.h"

static int des_cbc_handler(const char* str, size_t length) {
	const uint64_t	key = get_key();
	uint64_t		ciphertext,
					plaintext,
					iv;
	char*			base = NULL;
	char*			padded_str = NULL;

	if (!(g_des_flags & FLAG_INITVECTOR) || !g_initialization_vector) {
		ft_dprintf(STDERR_FILENO, "iv undefined\n");
		return (EXIT_FAILURE);
	}
	iv = create_64bit_chunk_from_hexstr(g_initialization_vector);
	if (g_des_flags & FLAG_SHOW_KEY)
		ft_dprintf(STDERR_FILENO, "iv =%016lX\n", iv);

	if (!(g_des_flags & FLAG_NO_PADDING) && g_des_flags & FLAG_ENCRYPT) {
		const uint8_t pad_amount = 8 - (length % 8);
		padded_str = ft_calloc(length + pad_amount, sizeof(char));
		ft_strlcpy(padded_str, str, length + 1);

		for (uint8_t i = 0; i < pad_amount; i++) {
			padded_str[length + i] = (char)pad_amount;
		}
		str = padded_str;
		length += pad_amount;
	}

	if (g_des_flags & FLAG_DECRYPT) {
		if (g_des_flags & FLAG_BASE64) {
			size_t newdatalen;
			base = base64_decode_string(str, length, &newdatalen);
			str = base;
			length = newdatalen;
		}
		for (size_t i = 0; i < length; i += CHUNK_SIZE_IN_BYTES) {
			ciphertext = REV64(*(uint64_t *)(str + i));

			plaintext = apply_des(ciphertext, key) ^ iv;
			add_chunk_to_buffer(plaintext, false);
//			ft_dprintf(STDERR_FILENO, "Decrypt: ciphertext = %016lX, plaintext = %016lX\n", ciphertext, plaintext);
			iv = ciphertext;
		}

	} else { // FLAG_ENCRYPT
		for (size_t i = 0; i < length; i += CHUNK_SIZE_IN_BYTES) {
			plaintext = create_64bit_chunk_from_str(str + i);

			ciphertext = apply_des(plaintext ^ iv, key);
			add_chunk_to_buffer(ciphertext, true);
//			ft_dprintf(STDERR_FILENO, "Encrypt: ciphertext = %016lX, plaintext = %016lX\n", ciphertext, plaintext);
			iv = ciphertext;
		}
	}

	if (!(g_des_flags & FLAG_NO_PADDING)) {
		free(padded_str);
	}

	clear_buffer(g_outfd, true);
	if (g_des_flags & FLAG_BASE64 && g_des_flags & FLAG_ENCRYPT) {
		ft_dprintf(g_outfd, "\n");
	}
	free(base);
	return (EXIT_SUCCESS);
}

int des_cbc_fd(const int fd) {
	int return_status;
	struct stat buf;
	char* file;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		ft_dprintf(STDERR_FILENO, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		ft_dprintf(STDERR_FILENO, "Error reading file.\n");
		return (EXIT_FAILURE);
	}
	return_status = des_cbc_handler(file, buf.st_size);
	munmap(file, buf.st_size);
	return (return_status);
}

int des_cbc_string(const char* str) {
	return (des_cbc_handler(str, ft_strlen(str)));
}
