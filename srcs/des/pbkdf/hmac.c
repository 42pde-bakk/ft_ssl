//
// Created by pde-bakk on 11/23/22.
//

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "libft.h"
#include "sha/sha256.h"

#define BLOCK_SIZE 64 // in bytes

/*
 * https://en.wikipedia.org/wiki/HMAC
 */

typedef t_sha2Context (*digest_func)(const char*, size_t);
static const digest_func digest = sha256_return_string;

static char *compute_block_sized_key(char *password) {
	char *block_sized_key = ft_calloc(BLOCK_SIZE + 1, sizeof(char));
	const size_t password_length = ft_strlen(password);

	if (!block_sized_key) {
		return (NULL);
	}
	if (password_length > BLOCK_SIZE) {
		t_sha2Context sha2Context = digest(password, password_length);
		password = ft_memcpy(password, sha2Context.digest, sizeof(sha2Context.digest));

		ft_memcpy(block_sized_key, password, SHA256_DIGEST_SIZE);
	}
	else {
		ft_memcpy(block_sized_key, password, password_length);
	}
	return (block_sized_key);
}

unsigned char * hmac(char *pass, const char *msg, size_t msg_length) {
	char* block_sized_key;
	t_sha2Context sha2Context;
	char	*padded_msg = ft_calloc(BLOCK_SIZE + msg_length + 1, sizeof(char));
	char	*hashed_padded_msg = ft_calloc(SHA256_DIGEST_SIZE + 1, sizeof(char));
	unsigned char	*result = ft_calloc(SHA256_DIGEST_SIZE, sizeof(char));

	if (!padded_msg || !hashed_padded_msg || !result) {
		free(padded_msg);
		free(hashed_padded_msg);
		free(result);
		return (NULL);
	}
	block_sized_key = compute_block_sized_key(pass);
	if (!block_sized_key) {
		free(padded_msg);
		free(hashed_padded_msg);
		free(result);
		return (NULL);
	}
	for (size_t i = 0; i < BLOCK_SIZE; i++) {
		padded_msg[i] = (char)(block_sized_key[i] ^ 0x36);
	}
	for (size_t i = 0; i < msg_length; i++) {
		padded_msg[BLOCK_SIZE + i] = msg[i];
	}

	sha2Context = digest(padded_msg, BLOCK_SIZE + msg_length);
	free(padded_msg);
	hashed_padded_msg = ft_memcpy(hashed_padded_msg, sha2Context.digest, SHA256_DIGEST_SIZE);
	padded_msg = ft_calloc(BLOCK_SIZE + SHA256_DIGEST_SIZE + 1, sizeof(char));
	for (size_t i = 0; i < BLOCK_SIZE; i++) {
		padded_msg[i] = (char)(block_sized_key[i] ^ 0x5c);
	}
	for (size_t i = 0; i < SHA256_DIGEST_SIZE; i++) {
		padded_msg[BLOCK_SIZE + i] = hashed_padded_msg[i];
	}
	sha2Context = digest(padded_msg, BLOCK_SIZE + SHA256_DIGEST_SIZE);
	ft_memcpy(result, sha2Context.digest, SHA256_DIGEST_SIZE);
	free(padded_msg);
	free(hashed_padded_msg);
	free(block_sized_key);
	return (result);
}
