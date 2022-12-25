//
// Created by pde-bakk on 11/23/22.
//

#include "libft.h"
#include "md5/md5.h"
#include "sha/sha256.h"
#include "des/des.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Change these values to md5 values to match the openssl -md md5 flag.
 */
typedef t_sha2Context digest_output;

typedef t_sha2Context (* digest_func)(const char*, size_t);

static const digest_func digest = sha256_return_string;
static const size_t OUTPUT_LENGTH = SHA256_DIGEST_SIZE;

void
pbkdf_1(const char* pass, uint64_t salt, const size_t total_iterations, uint64_t* output_key, uint64_t* output_iv) {
	digest_output data;
	const size_t len = ft_strlen(pass);
	const size_t total_length = len + 8 + OUTPUT_LENGTH;
	char* block = ft_calloc(total_length + 1, sizeof(char));

	if (!block) {
		perror("malloc failed");
		exit(1);
	}

	salt = REV64(salt);
	ft_bzero(&data, sizeof(data));
	ft_memcpy(block, pass, len);
	ft_memcpy(block + len, &salt, 8);
	data = digest(block, len + 8);

	for (size_t iter_nb = 1; iter_nb < total_iterations; iter_nb++) {
		ft_memcpy(block, data.digest, OUTPUT_LENGTH);
		ft_memcpy(block + OUTPUT_LENGTH, pass, len);
		ft_memcpy(block + OUTPUT_LENGTH + len, &salt, 8);
		data = digest(block, total_length);
	}
	*output_key = REV64(*(uint64_t*) data.digest);
	if (output_iv) {
		*output_iv = REV64(*(uint64_t*) (data.digest + 8));
	}
	free(block);
}
