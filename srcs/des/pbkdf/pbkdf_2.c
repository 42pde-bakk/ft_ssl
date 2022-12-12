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
	char	padded_salt[12];
	char	block[SHA256_DIGEST_SIZE];
	char	*hmac_result;
	char	*hmac_result_2;
	size_t	blocks;
	size_t	pos = 0;
	char	*key = calloc(100, 1);

	printf("pass = %s, passlen=%zu, salt=%s (%016lX), saltlen=%zu, keylen = %zu\n", pass, ft_strlen(pass), (char*)&salt, salt, 8lu, keylen);
	blocks = keylen / SHA256_DIGEST_SIZE + 1;
	salt = REV64(salt);
//	if (keylen % SHA256_DIGEST_SIZE) {
//		blocks++;
//	}

	ft_memcpy(padded_salt, &salt, 8);
	printf("blocks = %zu, padded_salt = %016lX\n", blocks, *(uint64_t*)padded_salt);

	for (uint32_t i = 1; i <= blocks; i++) {
//		*((uint32_t*)(padded_salt + saltlen)) = little_endian() ? byteswap32(i) : i;
		*((uint32_t*)(padded_salt + 8)) = REV32(i);
		printf("padded_salt (with added i) = %08X\n", *(uint32_t *)(padded_salt+8));

		hmac_result = hmac(pass, padded_salt);
		printf("tmp = %s\n", hmac_result);
		if (!hmac_result) {
			return (-1);
		}
		ft_memcpy(block, hmac_result, SHA256_DIGEST_SIZE);

		(void)iter;
		for (size_t j = 2; j <= 2; j++) {
			hmac_result_2 = hmac(pass, hmac_result);
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
	printf("key = %016lX\n", *(uint64_t *)key);
	(void)iv;
	(void)kkkey;
	return (0);
}
