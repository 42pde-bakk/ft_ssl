//
// Created by Peer de Bakker on 7/4/22.
//

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "md5/md5.h"
#include "md5/utils.h"

static void	md5_init(t_MD5Context *md5Context) {
	bzero(md5Context, sizeof(t_MD5Context));
	for (uint8_t i = 0; i < 4; ++i) {
		md5Context->buffer[i] = abcd[i];
	}
}

static void	md5_step(t_MD5Context *md5Context, const uint32_t *input) {
	uint32_t	A = md5Context->buffer[0],
				B = md5Context->buffer[1],
				C = md5Context->buffer[2],
				D = md5Context->buffer[3];

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
	md5Context->buffer[0] += A;
	md5Context->buffer[1] += B;
	md5Context->buffer[2] += C;
	md5Context->buffer[3] += D;
}

static void	md5_update(t_MD5Context *md5Context, const uint8_t *input_buffer, const size_t buffer_len) {
	uint32_t	input[MD5_DIGEST_LENGTH];
	uint32_t	offset = md5Context->size % 64;
	md5Context->size += buffer_len;

	for (uint64_t i = 0; i < buffer_len; ++i) {
		md5Context->input[offset] = input_buffer[i];
		offset++;

		if (offset % 64 == 0) {
			for (uint32_t j = 0; j < MD5_DIGEST_LENGTH; ++j) {
				input[j] = four_chars_to_uint32_md5(&md5Context->input[j * 4]);
			}
			md5_step(md5Context, input);
			offset = 0;
		}
	}
}

static void	md5_finalize(t_MD5Context *md5Context) {
	uint32_t	input[MD5_DIGEST_LENGTH];
	uint32_t	offset = md5Context->size % 64;
	uint32_t	padding_len;

	if (offset < 56) {
		padding_len = 56 - offset;
	} else {
		padding_len = (56 + 64) - offset;
	}
	md5_update(md5Context, padding, padding_len);
	md5Context->size -= padding_len;

	for (uint8_t i = 0; i < MD5_DIGEST_LENGTH - 2; i++) {
		input[i] = four_chars_to_uint32_md5(&md5Context->input[i * 4]);
	}
	input[MD5_DIGEST_LENGTH - 2] = md5Context->size * 8;
	input[MD5_DIGEST_LENGTH - 1] = (md5Context->size * 8) >> 32;

	md5_step(md5Context, input);
	for (uint8_t i = 0; i < 4; i++) {
		md5Context->digest[(i * 4) + 0] = (uint8_t)((md5Context->buffer[i] & 0x000000FF));
		md5Context->digest[(i * 4) + 1] = (uint8_t)((md5Context->buffer[i] & 0x0000FF00) >>  8);
		md5Context->digest[(i * 4) + 2] = (uint8_t)((md5Context->buffer[i] & 0x00FF0000) >> 16);
		md5Context->digest[(i * 4) + 3] = (uint8_t)((md5Context->buffer[i] & 0xFF000000) >> 24);
	}
}

int md5sum_string(const char* str) {
	t_MD5Context	md5Context;

	md5_init(&md5Context);
	md5_update(&md5Context, (uint8_t *)str, strlen(str));
	md5_finalize(&md5Context);

	print_hash(md5Context.digest, MD5_DIGEST_LENGTH);
	return (EXIT_SUCCESS);
}

int md5sum_file(int fd) {
	t_MD5Context	md5Context;
	ssize_t ret;
	char	BUF[MD5_BLOCK_SIZE + 1];

	md5_init(&md5Context);
	bzero(BUF, sizeof(BUF));
	while ((ret = read(fd, BUF, MD5_BLOCK_SIZE)) > 0) {
		md5_update(&md5Context, (uint8_t *)BUF, ret);
	}
	md5_finalize(&md5Context);

	print_hash(md5Context.digest, MD5_DIGEST_LENGTH);
	return (EXIT_SUCCESS);
}
