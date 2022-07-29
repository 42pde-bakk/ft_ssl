//
// Created by Peer de Bakker on 7/8/22.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "sha256.h"
#include "utils.h"

static const uint32_t	h[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
};

static const uint32_t k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void	sha256_init(t_sha256Context *sha256Context) {
	bzero(sha256Context, sizeof(t_sha256Context));
	for (unsigned int i = 0; i < 8; i++) {
		sha256Context->h[i] = h[i];
	}
}

void	sha256_transform(t_sha256Context *sha256Context) {
	uint32_t hash[8];
	uint32_t w[64];

	for (unsigned int i = 0; i < 8; i++) {
		hash[i] = sha256Context->h[i];
	}
	for (unsigned int i = 0; i < 64; i++) {
		if (i < 16) {
			// copy chunk into first 16 words w[0..15] of the message schedule array
			w[i] = four_chars_to_uint32(&sha256Context->data[i * 4]);
		} else {
			// Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
			const uint32_t s0 = rightrotate(w[i - 15], 7) ^ rightrotate(w[i - 15], 18) ^ (w[i - 15] >> 3);
			const uint32_t s1 = rightrotate(w[i - 2], 17) ^ rightrotate(w[i - 2], 19) ^ (w[i - 2] >> 10);
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}
	}

	// Compression function main loop:
	for (unsigned int i = 0; i < 64; i++) {
		uint32_t S1 = rightrotate(hash[4], 6) ^ rightrotate(hash[4], 11) ^ rightrotate(hash[4], 25);
		uint32_t ch = (hash[4] & hash[5]) ^ ((~hash[4]) & hash[6]);
		uint32_t temp1 = hash[7] + S1 + ch + k[i] + w[i];
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

void	sha256_update(t_sha256Context *sha256Context, const uint8_t *input_buffer, const size_t buffer_len) {
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

char	*sha256_finalize(t_sha256Context *sha256Context) {
	char *digest = calloc(SHA256_DIGEST_SIZE, sizeof(char));

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
		digest[i]      = (sha256Context->h[0] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 4]  = (sha256Context->h[1] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 8]  = (sha256Context->h[2] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 12] = (sha256Context->h[3] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 16] = (sha256Context->h[4] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 20] = (sha256Context->h[5] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 24] = (sha256Context->h[6] >> (24 - i * 8)) & 0x000000ff;
		digest[i + 28] = (sha256Context->h[7] >> (24 - i * 8)) & 0x000000ff;
	}
	return (digest);
}

int sha256_string(const char* str, char **save_digest) {
	t_sha256Context sha256Context;
	char	*digest;

	sha256_init(&sha256Context);
	sha256_update(&sha256Context, (uint8_t *)str, strlen(str));
	for (unsigned int i = 0; i < 8; i++) {
		fprintf(stderr, "h[%u] = %u\n", i, sha256Context.h[i]);
	}
	digest = sha256_finalize(&sha256Context);
	if (!digest)
		return (EXIT_FAILURE);
	*save_digest = digest;
	return (EXIT_SUCCESS);
}

int sha256_file(int fd, char **save_digest) {
	t_sha256Context sha256Context;
	ssize_t ret;
	char	BUF[SHA256_BLOCK_SIZE + 1];
	char	*digest;

	sha256_init(&sha256Context);
	bzero(BUF, sizeof(BUF));

	while ((ret = read(fd, BUF, SHA256_BLOCK_SIZE)) > 0) {
		sha256_update(&sha256Context, (uint8_t *)BUF, ret);
//		dprintf(2, "main. buffer = [%u, %u, %u, %u]\n", md5Context.buffer[0], md5Context.buffer[1], md5Context.buffer[2], md5Context.buffer[3]);
	}
	digest = sha256_finalize(&sha256Context);
	if (!digest)
		return (EXIT_FAILURE);
	*save_digest = digest;
	return (EXIT_SUCCESS);
}