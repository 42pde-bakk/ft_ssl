//
// Created by Peer de Bakker on 7/4/22.
//

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "md5.h"

/*
 * Rotates a 32-bit word left by n bits
 */

uint32_t leftrotate(uint32_t x, uint32_t n) {
	return (x << n) | (x >> (32 - n));
}

void	md5_init(t_MD5Context *md5Context) {
	bzero(md5Context, sizeof(t_MD5Context));
	for (uint8_t i = 0; i < 4; ++i) {
		md5Context->buffer[i] = abcd[i];
	}
}

void	md5_step(t_MD5Context *md5Context, const uint32_t *input) {
	uint32_t	A = a0,
				B = b0,
				C = c0,
				D = d0;

	for (uint8_t i = 0; i < 64; i++) {
		uint32_t f, g;
		if (i <= 15) {
			f = F(B, C, D);
			g = i;
		} else if (i <= 31) {
			f = G(B, C, D);
			g = (5 * i + 1) % 16;
		} else if (i <= 47) {
			f = H(B, C, D);
			g = (3 * i + 5) % 16;
		} else {
			f = I(B, C, D);
			g = (7 * i) % 16;
		}
		f = f + A + k[i] + input[g];
		A = D;
		D = C;
		C = B;
		B = B + leftrotate(f, shiftAmounts[i]);
	}
	md5Context->buffer[0] = A;
	md5Context->buffer[1] = B;
	md5Context->buffer[2] = C;
	md5Context->buffer[3] = D;
}

void	md5_update(t_MD5Context *md5Context, const uint8_t *input_buffer, const size_t buffer_len) {
	uint32_t	input[DIGEST_LENGTH];
	uint32_t	offset = md5Context->size % 64;
	md5Context->size += buffer_len;

	for (uint64_t i = 0; i < buffer_len; ++i) {
		md5Context->input[offset] = input_buffer[i];
		offset++;

		if (offset % 64 == 0) {
			for (uint32_t j = 0; j < DIGEST_LENGTH; ++j) {
				input[j] = (uint32_t)(md5Context->input[(j * 4) + 3]) << 24 |
						   (uint32_t)(md5Context->input[(j * 4) + 2]) << 16 |
						   (uint32_t)(md5Context->input[(j * 4) + 1]) <<  8 |
						   (uint32_t)(md5Context->input[(j * 4)]);
			}
			md5_step(md5Context, input);
		}
	}
}

void	md5_finalize(t_MD5Context *md5Context) {
	uint32_t	input[DIGEST_LENGTH];
	uint32_t	offset = md5Context->size % 64;
	uint32_t	padding_len;

	if (offset < 56) {
		padding_len = 56 - offset;
	} else {
		padding_len = (56 + 64) - offset;
	}

	md5_update(md5Context, padding, padding_len);
	md5Context->size -= padding_len;

	for (uint8_t i = 0; i < DIGEST_LENGTH - 2; i++) {
		input[i] = (uint32_t)(md5Context->input[(i * 4) + 3]) << 24 |
				   (uint32_t)(md5Context->input[(i * 4) + 2]) << 16 |
				   (uint32_t)(md5Context->input[(i * 4) + 1]) <<  8 |
				   (uint32_t)(md5Context->input[(i * 4)]);
	}
	input[DIGEST_LENGTH - 2] = md5Context->size * 8;
	input[DIGEST_LENGTH - 1] = (md5Context->size * 8) >> 32;

	md5_step(md5Context, input);

	for (uint8_t i = 0; i < 4; i++) {
		md5Context->digest[(i * 4) + 0] = (uint8_t)((md5Context->buffer[i] & 0x000000FF));
		md5Context->digest[(i * 4) + 1] = (uint8_t)((md5Context->buffer[i] & 0x0000FF00) >>  8);
		md5Context->digest[(i * 4) + 2] = (uint8_t)((md5Context->buffer[i] & 0x00FF0000) >> 16);
		md5Context->digest[(i * 4) + 3] = (uint8_t)((md5Context->buffer[i] & 0xFF000000) >> 24);
	}
}

void print_hash(uint8_t *digest){
	for (uint8_t i = 0; i < DIGEST_LENGTH; i++) {
		printf("%02x", digest[i]);
	}
	printf("\n");
}

char	*md5_digest(char *string) {
	t_MD5Context	md5Context;
	char			*digest;

	md5_init(&md5Context);
	md5_update(&md5Context, (uint8_t *)string, strlen(string));
	md5_finalize(&md5Context);

	digest = calloc(DIGEST_LENGTH + 1, sizeof(uint8_t));
//	printf("malloced for %zu, %p, %s\n", (DIGEST_LENGTH + 1) * sizeof(uint8_t), ());
	memcpy(digest, md5Context.digest, DIGEST_LENGTH * sizeof(uint8_t));
	print_hash(md5Context.digest);

	return (digest);
}




int	md5sum_string(unsigned int flags, const char *str) {
	(void)flags;
	(void)str;
	const char string[] = "The quick brown fox jumped over the lazy dog's back";

	char	*digest = md5_digest((char *)string);
	printf("digest = %s\n", digest);
	free(digest);
	return (0);
}

int	md5sum_file(unsigned int flags, const char *filename) {
	(void)flags;
	return (md5sum_string(flags, filename));
//	int fd = open(str, O_RDONLY);
//	size_t len = lseek(str, 0, SEEK_END);
//	dprintf(2, "lseek = %zu\n", len);
//	void	*data = mmap(0, len, PROT_READ, MAP_PRIVATE | MAP_SHARED, str, 0);
//	if (data == MAP_FAILED) {
//		perror("mmap");
//		return (EXIT_FAILURE);
//	}
//	int ret = calc_md5sum((char *)data);
//	munmap(data, len);
//	return (ret);
}
