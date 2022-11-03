//
// Created by pde-bakk on 11/2/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include <string.h>
#include "des/des.h"
#include "des/flags.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>

char*	get_password() {
	char *pass = getpass("enter ft_des encryption password:");
	printf("pass = %s and strlen = %zu\n", pass, strlen(pass));
	return (pass);
}

int	des_encode_string(const char* str) {
	const size_t datalen = ft_strlen(str);
	char	*pass = get_password();
	(void)pass;
	(void)datalen;

	uint64_t	input = 0x9474B8E8C73BCA7D;
	uint64_t	key = 0x0000000000000000;
	uint64_t	key56 = produce_56bit_key(key);
	uint64_t	sub_keys[16];
	uint64_t	result = input;
	uint32_t	lpt, // Left Plain Text
				rpt; // Right Plain Text
	uint32_t	left_key,
				right_key;

	/*
	 * TESTING IMPLEMENTATION OF DES
	 * Ronald L. Rivest
	 * X0:  9474B8E8C73BCA7D
	 * X16: 1B1A2DDB4C642438
	 *
	 * OUTPUT:
	 * E: 8da744e0c94e5e17
	 * D: 0cdb25e3ba3c6d79
	 * E: 4784c4ba5006081f
	 * D: 1cf1fc126f2ef842
	 * E: e4be250042098d13
	 * D: 7bfc5dc6adb5797c
	 * E: 1ab3b4d82082fb28
	 * D: c1576a14de707097
	 * E: 739b68cd2e26782a
	 * D: 2a59f0c464506edb
	 * E: a5c39d4251f0a81e
	 * D: 7239ac9a6107ddb1
	 * E: 070cac8590241233
	 * D: 78f87b6e3dfecf61
	 * E: 95ec2578c2c433f0
	 * D: 1b1a2ddb4c642438  <-- X16
	 */

	result = perform_initial_permutation(input);
	msg_split_blocks(result, &lpt, &rpt);

	// Apply permutations to the keys too, because why the hell not?
	key_split_blocks(key, &left_key, &right_key);

	/* Key transformation */
	for (size_t i = 0; i < 16; i++){
		shift_key(&left_key, &right_key, i);

		const uint64_t	permuted_key_2 = (((uint64_t)left_key) << 28) | (uint64_t)right_key;
		sub_keys[i] = shift_sub_key(permuted_key_2);
	}

	for (size_t i = 0; i < 16; i++) {
		uint64_t	expanded_rpt = expand_rpt(rpt);

		// Time to XOR
		// this is the difference between encryption and decryption
		// Now the 48-bit key is XOR with 48-bit RPT and the resulting output is given to the next step
		// Which is the S-Box substitution.
		expanded_rpt ^= sub_keys[i];

		uint32_t s_output = 0;
		for (size_t j = 0; j < 8; j++) {
			s_output <<= 4;
			s_output |= get_sbox_output(expanded_rpt, i);
		}
	}

	// Inverse the initial permutations
//	perform_inverse_initial_permutation(pre_output);
	return (0);
}

int des_encode_file(const int fd) {
	struct stat buf;
	char* file;

	ft_memset(&buf, 0, sizeof(buf));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0 || S_ISDIR(buf.st_mode)) {
		fprintf(stderr, "Error opening file.\n");
		return (EXIT_FAILURE);
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "Error rading file.\n");
		return (EXIT_FAILURE);
	}
	des_encode_string(file);
	munmap(file, buf.st_size);
	return (EXIT_SUCCESS);
}
