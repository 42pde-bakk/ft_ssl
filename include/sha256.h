//
// Created by Peer De bakker on 7/29/22.
//

#ifndef FT_SSL_SHA256_H
#define FT_SSL_SHA256_H
#include <stdint.h>

#define SHA256_DIGEST_SIZE 32
#define SHA256_BLOCK_SIZE 256

typedef struct s_sha256Context {
	uint32_t 	h[8];
	uint8_t		data[64];
	uint32_t	data_len;
	uint64_t	bitlen;
}	t_sha256Context;

#endif //FT_SSL_SHA256_H
