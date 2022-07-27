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
#include <sys/stat.h>
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
	uint32_t	A = md5Context->buffer[0],
				B = md5Context->buffer[1],
				C = md5Context->buffer[2],
				D = md5Context->buffer[3];

	for (uint8_t i = 0; i < 64; i++) {

//		dprintf(2, "md5_step, i = %hhu\n", i);
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
//		dprintf(2, "f = %u, g = %u\n", f, g);
		f = f + A + k[i] + input[g];
		A = D;
		D = C;
		C = B;
		B = B + leftrotate(f, shiftAmounts[i]);
//		dprintf(2, "A = %u, B = %u, C = %u, D = %u\n", A, B, C, D);
	}
	md5Context->buffer[0] += A;
	md5Context->buffer[1] += B;
	md5Context->buffer[2] += C;
	md5Context->buffer[3] += D;
//	dprintf(2, "buffer = [%u, %u, %u, %u]\n", md5Context->buffer[0], md5Context->buffer[1], md5Context->buffer[2], md5Context->buffer[3]);
}

void	md5_update(t_MD5Context *md5Context, const uint8_t *input_buffer, const size_t buffer_len) {
	uint32_t	input[DIGEST_LENGTH];
	uint32_t	offset = md5Context->size % 64;
	md5Context->size += buffer_len;

	for (uint64_t i = 0; i < buffer_len; ++i) {
		md5Context->input[offset] = input_buffer[i];
//		dprintf(2, "ctx->input[%u] = %hhu\n", offset, md5Context->input[offset]);
		offset++;

		if (offset % 64 == 0) {
			for (uint32_t j = 0; j < DIGEST_LENGTH; ++j) {
				input[j] = (uint32_t)(md5Context->input[(j * 4) + 3]) << 24 |
						   (uint32_t)(md5Context->input[(j * 4) + 2]) << 16 |
						   (uint32_t)(md5Context->input[(j * 4) + 1]) <<  8 |
						   (uint32_t)(md5Context->input[(j * 4)]);
//				dprintf(2, "input[%u] = %u\n", j, input[j]);
			}
			md5_step(md5Context, input);
			offset = 0;
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
//	dprintf(2, "finalizing!\n");
//	dprintf(2, "offset = %u, padding_length = %u\n", offset, padding_len);

	md5_update(md5Context, padding, padding_len);
	md5Context->size -= padding_len;
//	dprintf(2, "size = %llu\n", md5Context->size);


	for (uint8_t i = 0; i < DIGEST_LENGTH - 2; i++) {
		input[i] = (uint32_t)(md5Context->input[(i * 4) + 3]) << 24 |
				   (uint32_t)(md5Context->input[(i * 4) + 2]) << 16 |
				   (uint32_t)(md5Context->input[(i * 4) + 1]) <<  8 |
				   (uint32_t)(md5Context->input[(i * 4)]);
//		dprintf(2, "input[%hhu] = %u\n", i, input[i]);
	}
	input[DIGEST_LENGTH - 2] = md5Context->size * 8;
	input[DIGEST_LENGTH - 1] = (md5Context->size * 8) >> 32;
//	dprintf(2, "input[%d] = %u\n", DIGEST_LENGTH - 2, input[DIGEST_LENGTH - 2]);
//	dprintf(2, "input[%d] = %u\n", DIGEST_LENGTH - 1, input[DIGEST_LENGTH - 1]);

	md5_step(md5Context, input);

	for (uint8_t i = 0; i < 4; i++) {
		md5Context->digest[(i * 4) + 0] = (uint8_t)((md5Context->buffer[i] & 0x000000FF));
		md5Context->digest[(i * 4) + 1] = (uint8_t)((md5Context->buffer[i] & 0x0000FF00) >>  8);
		md5Context->digest[(i * 4) + 2] = (uint8_t)((md5Context->buffer[i] & 0x00FF0000) >> 16);
		md5Context->digest[(i * 4) + 3] = (uint8_t)((md5Context->buffer[i] & 0xFF000000) >> 24);
	}
}

int md5sum_string(const char* str, char **save_digest) {
	t_MD5Context	md5Context;
	char			*digest;

	md5_init(&md5Context);
	md5_update(&md5Context, (uint8_t *)str, strlen(str));
//	dprintf(2, "size = %llu, buffer = [%u, %u, %u, %u]\n", md5Context.size, md5Context.buffer[0], md5Context.buffer[1], md5Context.buffer[2], md5Context.buffer[3]);
	md5_finalize(&md5Context);

	digest = calloc(DIGEST_LENGTH + 1, sizeof(uint8_t));
	if (!digest) {
		perror("malloc");
		return (EXIT_FAILURE);
	}
	memcpy(digest, md5Context.digest, DIGEST_LENGTH * sizeof(uint8_t));
	*save_digest = digest;
	return (EXIT_SUCCESS);
}

int md5sum_file(int fd, char **save_digest) {
	ssize_t ret;
	char	BUF[BLOCK_SIZE + 1];
	t_MD5Context	md5Context;
	char	*digest;

	digest = calloc(DIGEST_LENGTH, sizeof(uint8_t));
	bzero(BUF, sizeof(BUF));
	if (!digest) {
		perror("calloc");
		return (EXIT_FAILURE);
	}
	md5_init(&md5Context);
	while ((ret = read(fd, BUF, BLOCK_SIZE)) > 0) {
		md5_update(&md5Context, (uint8_t *)BUF, ret);
//		dprintf(2, "main. buffer = [%u, %u, %u, %u]\n", md5Context.buffer[0], md5Context.buffer[1], md5Context.buffer[2], md5Context.buffer[3]);
	}
	md5_finalize(&md5Context);

	memcpy(digest, md5Context.digest, DIGEST_LENGTH * sizeof(uint8_t));
	*save_digest = digest;
//	print_hash((uint8_t *)digest);
//	free(digest);
	return (EXIT_SUCCESS);
}
