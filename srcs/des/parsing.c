//
// Created by peer on 8-11-22.
//

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "des/flags.h"
#include "des/des.h"
#include "libft.h"
#include <sys/random.h>
#include <stdlib.h>

uint64_t	create_64bit_chunk_from_str(const char* const str) {
	size_t		stringlength = ft_strlen(str);
	uint64_t	chunk = 0;

	for (size_t i = 0; i < 8; i++) {
		chunk <<= 8;
		if (i < stringlength)
			chunk |= (uint64_t)str[i];
	}
	return (chunk);
}

uint64_t	create_64bit_chunk_from_hexstr(const char* const str) {
	size_t	datalen = ft_strlen(str);
	uint64_t hex;

	hex = strtoul(str, NULL, 16);
	while (datalen < 16) {
		hex <<= 4;
		++datalen;
	}
	return (hex);
}

static uint64_t	generate_random_salt() {
	uint64_t	bytes;

	getrandom(&bytes, 8, 0);
	return (bytes);
}

uint64_t	get_key() {
	uint64_t	key;
	uint64_t	salt;
	char		*pass;

	if (g_des_flags & FLAG_KEY && g_key != NULL) {
		key = create_64bit_chunk_from_hexstr(g_key);
		if (g_des_flags & FLAG_SHOW_KEY) {
			printf("key=%016lX\n", key);
		}
		return (key);
	}
	pass = getpass("enter des encryption password:");

	if (g_des_flags & FLAG_SALT && g_salt != NULL) {
		salt = create_64bit_chunk_from_hexstr(g_salt);
	} else {
		salt = generate_random_salt();
	}
	key = create_64bit_chunk_from_str(pass);
	key = pbkdf(key, salt);
	if (g_des_flags & FLAG_SHOW_KEY) {
		printf("salt=%016lX\n", salt);
		printf("key=%016lX\n", key);
	}
	return (key);
}
