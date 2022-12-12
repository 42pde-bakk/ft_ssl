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
#include "ft_printf.h"

uint64_t	create_64bit_chunk_from_str(const char* const str) {
	uint64_t	chunk = 0;
	bool		reached_end = false;

	for (size_t i = 0; i < 8; i++) {
		chunk <<= 8;
		if (!reached_end)
			chunk |= (uint64_t)str[i];
		if (str[i] == '\0') {
			reached_end = true;
		}
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

int get_key(uint64_t *key, uint64_t *iv) {
	uint64_t	salt;
	const char*	password;

	if (g_des_flags & FLAG_KEY && g_key != NULL) {
		*key = create_64bit_chunk_from_hexstr(g_key);
		if (g_des_flags & FLAG_SHOW_KEY) {
			dprintf(STDERR_FILENO, "key=%016lX\n", *key);
		}
		return (EXIT_SUCCESS);
	}
	if (g_des_flags & FLAG_PASSWORD && g_password != NULL) {
		password = g_password;
		*key = create_64bit_chunk_from_str(g_password);
	} else {
		password = getpass("enter des encryption password:");
		*key = create_64bit_chunk_from_str(password);
	}

	if (g_des_flags & FLAG_SALT && g_salt != NULL) {
		salt = create_64bit_chunk_from_hexstr(g_salt);
	} else {
		salt = generate_random_salt();
	}
	if (g_des_flags & FLAG_ENCRYPT) {
		add_chunk_to_buffer(create_64bit_chunk_from_str("Salted__"), true);
		add_chunk_to_buffer(salt, true);
	}

#if PBKDF_VERSION == 1
	pbkdf_1(password, salt, 1, key, iv);
#elif PBKDF_VERSION == 2
	pbkdf_2((char *)password, salt, 10000, key, iv);
#else
	perror("INVALID PBKDF VERSION");
	exit(1);
#endif
	if (g_des_flags & FLAG_SHOW_KEY) {
		dprintf(STDERR_FILENO,"salt=%016lX\n", salt);
		dprintf(STDERR_FILENO,"key=%016lX\n", *key);
		if (iv)
			dprintf(STDERR_FILENO,"iv=%016lX\n", *iv);
	}
	return (EXIT_SUCCESS);
}
