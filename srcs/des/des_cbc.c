//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include "des/flags.h"
#include "des/des.h"
#include "libft.h"
#include <sys/random.h>

void test();

int des_cbc_fd(const int fd) {
	const uint64_t	key = get_key();
	struct stat buf;
	char* file;
	uint64_t result = 0;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error rading file.\n");
		return (EXIT_FAILURE);
	}
	if (g_des_flags & FLAG_INITVECTOR && g_initialization_vector)
		result = create_64bit_chunk_from_hexstr(g_initialization_vector);

	if (g_des_flags & FLAG_SHOW_KEY)
		printf("iv= %016lX\n", result);
	for (size_t i = 0; i < buf.st_size; i += CHUNK_SIZE_IN_BYTES) {
		const uint64_t chunk = create_64bit_chunk_from_str(file + i);
		result = apply_des(chunk ^ result, key);
		printf("%016lX", result);
	}
	munmap(file, buf.st_size);
	return (EXIT_SUCCESS);
}

int des_cbc_string(const char* str) {
	const size_t datalen = ft_strlen(str);
	const uint64_t	key = get_key();
	uint64_t	result = 0;

	if (g_des_flags & FLAG_INITVECTOR && g_initialization_vector)
		result = create_64bit_chunk_from_hexstr(g_initialization_vector);
	if (g_des_flags & FLAG_SHOW_KEY)
		printf("iv= %016lX\n", result);

	for (size_t i = 0; i < datalen; i += CHUNK_SIZE_IN_BYTES) {
		const uint64_t chunk = create_64bit_chunk_from_str(str + i);
		result = apply_des(chunk ^ result, key);
		printf("%016lX", result);
	}
	return (EXIT_SUCCESS);
}
