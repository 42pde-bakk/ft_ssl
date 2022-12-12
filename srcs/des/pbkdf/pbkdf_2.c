//
// Created by pde-bakk on 11/23/22.
//

#include "libft.h"
#include "des/des.h"
#include "sha/sha256.h"
#include <stdio.h>
#include <stdlib.h>

static const size_t keylen = 16;

int pbkdf_2(char *pass, uint64_t salt, size_t iter, uint64_t *kkkey, uint64_t *iv) {
	char			padded_salt[12];
	unsigned char	block[SHA256_DIGEST_SIZE];
	unsigned char	*hmac_result;
	unsigned char	*hmac_result_2;
	size_t	blocks;
	size_t	pos = 0;
	char	*key = calloc(32, 1);

	blocks = keylen / SHA256_DIGEST_SIZE + 1;
	salt = REV64(salt);

	ft_memcpy(padded_salt, &salt, 8);

	for (uint32_t i = 1; i <= blocks; i++) {
		*((uint32_t*)(padded_salt + 8)) = REV32(i);

		hmac_result = hmac(pass, padded_salt, 12);
		if (!hmac_result) {
			return (-1);
		}
		ft_memcpy(block, hmac_result, SHA256_DIGEST_SIZE);

		for (size_t j = 2; j <= iter; j++) {
			hmac_result_2 = hmac(pass, (char *)hmac_result, SHA256_DIGEST_SIZE);
			free(hmac_result);
			if (!hmac_result_2) {
				return (-1);
			}
			for (size_t k = 0; k < SHA256_DIGEST_SIZE; k++) {
				block[k] ^= hmac_result_2[k];
			}
			hmac_result = hmac_result_2;
			hmac_result_2 = NULL;
		}
		free(hmac_result);

		if (pos + SHA256_DIGEST_SIZE > keylen) {
			ft_memcpy(key + pos, block, (keylen - pos));
		}
		else {
			ft_memcpy(key + pos, block, SHA256_DIGEST_SIZE);
		}
		pos += SHA256_DIGEST_SIZE;
	}
	*kkkey = REV64(*(uint64_t *)key);
	if (iv) {
		*iv = REV64(*(uint64_t *)(key + 8));
	}
	free(key);
	return (0);
}
