//
// Created by Peer De bakker on 7/29/22.
//

#ifndef FT_SSL_SHA256_H
#define FT_SSL_SHA256_H
#include <stdint.h>

#define SHA256_DIGEST_SIZE 32
#define SHA224_DIGEST_SIZE 28

#define SHA256_BLOCK_SIZE 256 // in bits

typedef struct s_sha2Context {
	uint32_t 	h[8];
	uint8_t		data[64];
	uint32_t	data_len;
	uint64_t	bitlen;
	uint8_t		digest[SHA256_DIGEST_SIZE];
}	t_sha2Context;

extern const uint32_t	sha256_h[8];
extern const uint32_t	sha224_h[8];
extern const uint32_t k_sha2[64];

// shared/s0_s1.c
uint32_t get_s0(uint32_t item);
uint32_t get_s1(uint32_t item);

t_sha2Context sha256_return_string(const char *str, size_t length);

#endif //FT_SSL_SHA256_H
