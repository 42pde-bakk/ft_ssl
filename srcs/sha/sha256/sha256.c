//
// Created by Peer de Bakker on 7/8/22.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "sha256.h"
#include "utils.h"

void sha256_init(t_sha2Context* sha256Context, const uint32_t* hash_values) {
	bzero(sha256Context, sizeof(t_sha2Context));
	for (unsigned int i = 0; i < 8; i++) {
		sha256Context->h[i] = hash_values[i];
	}
}

void	sha256_transform(t_sha2Context *sha256Context) {
	uint32_t hash[8];
	uint32_t w[64];

	for (unsigned int i = 0; i < 8; i++) {
		hash[i] = sha256Context->h[i];
	}
	for (unsigned int i = 0; i < 64; i++) {
		if (i < 16) {
			// copy chunk into first 16 words w[0..15] of the message schedule array
			w[i] = four_chars_to_uint32_sha256(&sha256Context->data[i * 4]);
//			printf("w[%u] = %#x\n", i, w[i]);
		} else {
			// Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
			const uint32_t s0 = get_s0(w[i - 15]);
			const uint32_t s1 = get_s1(w[i - 2]);
//			printf("w[i] = %#x + %#x + %#x + %#x\n",  w[i - 16], s0, w[i - 7], s1);

			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
//			printf("s0 = %#x, s1 = %#x, w[%u] = %#x\n", s0, s1, i, w[i]);
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

void	sha256_update(t_sha2Context *sha256Context, const uint8_t *input_buffer, const size_t buffer_len) {
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

void	sha256_finalize(t_sha2Context *sha256Context) {
//	char *digest = calloc(SHA256_DIGEST_SIZE, sizeof(char));

	uint32_t i;

	i = sha256Context->data_len;

	// Pad whatever data is left in the buffer.
	if (sha256Context->data_len < 56) {
		sha256Context->data[i] = 0x80;
		i++;
		while (i < 56) {
			sha256Context->data[i] = 0x00;
			i++;
		}
	}
	else {
		sha256Context->data[i] = 0x80;
		i++;
		while (i < 64)
			sha256Context->data[i++] = 0x00;
		sha256_transform(sha256Context);
		memset(sha256Context->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	sha256Context->bitlen += sha256Context->data_len * 8;
	sha256Context->data[63] = sha256Context->bitlen;
	sha256Context->data[62] = sha256Context->bitlen >> 8;
	sha256Context->data[61] = sha256Context->bitlen >> 16;
	sha256Context->data[60] = sha256Context->bitlen >> 24;
	sha256Context->data[59] = sha256Context->bitlen >> 32;
	sha256Context->data[58] = sha256Context->bitlen >> 40;
	sha256Context->data[57] = sha256Context->bitlen >> 48;
	sha256Context->data[56] = sha256Context->bitlen >> 56;
	sha256_transform(sha256Context);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		sha256Context->digest[i]      = (int8_t)((sha256Context->h[0] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 4]  = (int8_t)((sha256Context->h[1] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 8]  = (int8_t)((sha256Context->h[2] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 12] = (int8_t)((sha256Context->h[3] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 16] = (int8_t)((sha256Context->h[4] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 20] = (int8_t)((sha256Context->h[5] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 24] = (int8_t)((sha256Context->h[6] >> (24 - i * 8)) & 0x000000ff);
		sha256Context->digest[i + 28] = (int8_t)((sha256Context->h[7] >> (24 - i * 8)) & 0x000000ff);
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
