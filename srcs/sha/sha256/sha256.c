//
// Created by Peer de Bakker on 7/8/22.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "sha/sha256.h"
#include "sha/utils.h"

static void sha256_init(t_sha2Context* sha256Context, const uint32_t* hash_values) {
	bzero(sha256Context, sizeof(t_sha2Context));
	for (unsigned int i = 0; i < 8; i++) {
		sha256Context->h[i] = hash_values[i];
	}
}

static void	sha256_transform(t_sha2Context *sha256Context) {
	uint32_t hash[8];
	uint32_t w[64];

	for (unsigned int i = 0; i < 8; i++) {
		hash[i] = sha256Context->h[i];
	}
	for (unsigned int i = 0; i < 64; i++) {
		if (i < 16) {
			// copy chunk into first 16 words w[0..15] of the message schedule array
			w[i] = four_chars_to_uint32_sha256(&sha256Context->data[i * 4]);
		} else {
			// Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
			const uint32_t s0 = get_s0(w[i - 15]);
			const uint32_t s1 = get_s1(w[i - 2]);

			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}
	}

	// Compression function main loop:
	for (unsigned int i = 0; i < 64; i++) {
		uint32_t S1 = rightrotate(hash[4], 6) ^ rightrotate(hash[4], 11) ^ rightrotate(hash[4], 25);
		uint32_t ch = (hash[4] & hash[5]) ^ ((~hash[4]) & hash[6]);
		uint32_t temp1 = hash[7] + S1 + ch + k_sha2[i] + w[i];
		uint32_t S0 = rightrotate(hash[0], 2) ^ rightrotate(hash[0], 13) ^ rightrotate(hash[0], 22);
		uint32_t maj = (hash[0] & hash[1]) ^ (hash[0] & hash[2]) ^ (hash[1] & hash[2]);
		uint32_t temp2 = S0 + maj;

		hash[7] = hash[6];
		hash[6] = hash[5];
		hash[5] = hash[4];
		hash[4] = hash[3] + temp1;
		hash[3] = hash[2];
		hash[2] = hash[1];
		hash[1] = hash[0];
		hash[0] = temp1 + temp2;
	}

	// Add the compressed chunk to the current hash value:
	for (unsigned int i = 0; i < 8; i++) {
		sha256Context->h[i] += hash[i];
	}
}

static void	sha256_update(t_sha2Context *sha256Context, const uint8_t *input_buffer, const size_t buffer_len) {
	for (unsigned int i = 0; i < buffer_len; i++) {
		sha256Context->data[sha256Context->data_len] = input_buffer[i];
		sha256Context->data_len++;

		if (sha256Context->data_len == 64) {
			sha256_transform(sha256Context);
			sha256Context->bitlen += 512;
			sha256Context->data_len = 0;
		}
	}
}

static void	sha256_finalize(t_sha2Context *sha256Context) {
	uint32_t i;
	const uint32_t padding_length = (sha256Context->data_len < 56) ? 56 : 64;

	i = sha256Context->data_len;
	sha256Context->data[i] = 0x80;
	i++;

	// Pad whatever data is left in the buffer.
	while (i < padding_length) {
		sha256Context->data[i] = 0x00;
		i++;
	}
	if (sha256Context->data_len >= 56) {
		// We do this because we need some space to store the total message's length
		sha256_transform(sha256Context);
		memset(sha256Context->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	sha256Context->bitlen += sha256Context->data_len * 8; // 8 bits per byte
	for (i = 0; i < 8; i++) {
		const uint32_t bitshift = 8 * i;
		sha256Context->data[63 - i] = (uint8_t)(sha256Context->bitlen >> bitshift);
	}

	sha256_transform(sha256Context);

	// Reverse the bytes because it wants it in big endian, and we use little endian
	for (i = 0; i < 4; ++i) {
		const uint32_t bitshift = 24 - i * 8;
		for (uint32_t n = 0; n < 8; n++) {
			sha256Context->digest[i + 4 * n] = (uint8_t)((sha256Context->h[n] >> bitshift) & 0xff);
		}
	}
}

int sha256_string(const char* str) {
	t_sha2Context sha256Context;

	sha256_init(&sha256Context, sha256_h);
	sha256_update(&sha256Context, (uint8_t *)str, strlen(str));

	sha256_finalize(&sha256Context);
	print_hash(sha256Context.digest, SHA256_DIGEST_SIZE);
	return (EXIT_SUCCESS);
}

int sha256_file(int fd) {
	t_sha2Context sha256Context;
	ssize_t ret;
	char	BUF[SHA256_BLOCK_SIZE + 1];

	sha256_init(&sha256Context, sha256_h);
	bzero(BUF, sizeof(BUF));

	while ((ret = read(fd, BUF, SHA256_BLOCK_SIZE)) > 0) {
		sha256_update(&sha256Context, (uint8_t *)BUF, ret);
	}

	sha256_finalize(&sha256Context);
	print_hash(sha256Context.digest, SHA256_DIGEST_SIZE);
	return (EXIT_SUCCESS);
}

int sha224_string(const char* str) {
	t_sha2Context sha256Context;

	sha256_init(&sha256Context, sha224_h);
	sha256_update(&sha256Context, (uint8_t *)str, strlen(str));

	sha256_finalize(&sha256Context);
	print_hash(sha256Context.digest, SHA224_DIGEST_SIZE);
	return (EXIT_SUCCESS);
}

int sha224_file(int fd) {
	t_sha2Context sha256Context;
	ssize_t ret;
	char	BUF[SHA256_BLOCK_SIZE + 1];

	sha256_init(&sha256Context,sha224_h);
	bzero(BUF, sizeof(BUF));

	while ((ret = read(fd, BUF, SHA256_BLOCK_SIZE)) > 0) {
		sha256_update(&sha256Context, (uint8_t *)BUF, ret);
	}

	sha256_finalize(&sha256Context);
	print_hash(sha256Context.digest, SHA224_DIGEST_SIZE);
	return (EXIT_SUCCESS);
}
