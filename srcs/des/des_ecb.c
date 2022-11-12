//
// Created by peer on 8-11-22.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include <assert.h>
#include "des/flags.h"
#include "des/des.h"
#include "libft.h"
#include "base64/base64.h"

static int des_ecb_handler(const char* str, size_t length) {
	const uint64_t	key = get_key();
	uint64_t		ciphertext,
					plaintext;
	char*			base = NULL;
	char*			padded_str = NULL;

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

			plaintext = apply_des(ciphertext, key);
//			dprintf(STDERR_FILENO, "Decrypt: ciphertext = %016lX, plaintext = %016lX\n", ciphertext, plaintext);
			add_chunk_to_buffer(plaintext, false);
		}

	} else { // FLAG_ENCRYPT
		for (size_t i = 0; i < length; i += CHUNK_SIZE_IN_BYTES) {
			plaintext = create_64bit_chunk_from_str(str + i);

			ciphertext = apply_des(plaintext, key);
//			dprintf(STDERR_FILENO, "Encrypt: ciphertext = %016lX, plaintext = %016lX\n", ciphertext, plaintext);
			add_chunk_to_buffer(ciphertext, true);
		}
	}

	if (!(g_des_flags & FLAG_NO_PADDING)) {
		free(padded_str);
	}

	clear_buffer(g_outfd, true);
	free(base);
	return (EXIT_SUCCESS);
}

int des_ecb_fd(const int fd) {
	struct stat	buf;
	char		*file;
	int			return_status;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error reading file.\n");
		return (EXIT_FAILURE);
	}
	return_status = des_ecb_handler(file, buf.st_size);
	dprintf(g_outfd, "\n");

	munmap(file, buf.st_size);
	return (return_status);
}

int des_ecb_string(const char* str) {
	return (des_ecb_handler(str, ft_strlen(str)));
}


/*
	* TESTING IMPLEMENTATION OF DES
	* Ronald L. Rivest
	* X0:  9474B8E8C73BCA7D
	* X16: 1B1A2DDB4C642438
	*
	* OUTPUT:
	* E: 8da744e0c94e5e17
	* D: 0cdb25e3ba3c6d79
	* E: 4784c4ba5006081f
	* D: 1cf1fc126f2ef842
	* E: e4be250042098d13
	* D: 7bfc5dc6adb5797c
	* E: 1ab3b4d82082fb28
	* D: c1576a14de707097
	* E: 739b68cd2e26782a
	* D: 2a59f0c464506edb
	* E: a5c39d4251f0a81e
	* D: 7239ac9a6107ddb1
	* E: 070cac8590241233
	* D: 78f87b6e3dfecf61
	* E: 95ec2578c2c433f0
	* D: 1b1a2ddb4c642438  <-- X16
*/
void	test() {
	const uint64_t	expected_outcomes[16] = {
			0x8da744e0c94e5e17,
			0x0cdb25e3ba3c6d79,
			0x4784c4ba5006081f,
			0x1cf1fc126f2ef842,
			0xe4be250042098d13,
			0x7bfc5dc6adb5797c,
			0x1ab3b4d82082fb28,
			0xc1576a14de707097,
			0x739b68cd2e26782a,
			0x2a59f0c464506edb,
			0xa5c39d4251f0a81e,
			0x7239ac9a6107ddb1,
			0x070cac8590241233,
			0x78f87b6e3dfecf61,
			0x95ec2578c2c433f0,
			0x1b1a2ddb4c642438
	};
	const unsigned int flags_backup = g_des_flags;
	uint64_t res = 0x9474B8E8C73BCA7D;
	printf("\n");
	for (size_t i = 0; i < 16; i++) {
		g_des_flags = 0;
		if (i % 2 == 0) {
			g_des_flags |= FLAG_ENCRYPT;
			res = apply_des(res, res);
			printf("E: %016lx\n", res);
		}
		else {
			g_des_flags |= FLAG_DECRYPT;
			res = apply_des(res, res);
			printf("D: %016lx\n", res);
		}
		assert(res == expected_outcomes[i]);
	}
	g_des_flags = flags_backup;
}
