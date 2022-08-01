//
// Created by Peer De bakker on 7/23/22.
//

#ifndef FT_SSL_MD5_H
#define FT_SSL_MD5_H

#include <stdint.h>
#include <stddef.h>

#define MD5_DIGEST_LENGTH 16
#define BLOCK_SIZE 512

#define F(B, C, D) ((B & C) | (~B & D)) //f0
#define G(B, C, D) ((B & D) | (C & ~D))	//f1
#define H(B, C, D) (B ^ C ^ D)			//f2
#define I(B, C, D) (C ^ (B | ~D))		//f3

typedef struct s_MD5Context {
	uint64_t	size;
	uint32_t	count[2];
	uint32_t	buffer[4];
	uint8_t		input[64];
	uint8_t		digest[MD5_DIGEST_LENGTH];
}	t_MD5Context;


extern const uint32_t shiftAmounts[64];
extern const uint32_t	k[64];
extern const uint8_t padding[64];

extern const uint32_t a0;
extern const uint32_t b0;
extern const uint32_t c0;
extern const uint32_t d0;
extern const uint32_t abcd[4];

#endif //FT_SSL_MD5_H
