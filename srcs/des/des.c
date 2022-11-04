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

uint64_t apply_des(uint64_t input, uint64_t key, char mode) {
	uint64_t	key56 = produce_56bit_key(key);
	uint64_t	sub_keys[16];
	uint64_t	result;
	uint32_t	lpt, // Left Plain Text
				rpt; // Right Plain Text
	uint32_t	left_key,
				right_key;
	result = perform_initial_permutation(input);
//	printf("init_perm_res = %lX\n", result);
	msg_split_blocks(result, &lpt, &rpt);
//	printf("lpt=%X, rpt=%X\n", lpt, rpt);

	// Apply permutations to the keys too, because why the hell not?
	key_split_blocks(key56, &left_key, &right_key);
//	printf("key56 = %lX, leftkey=%X, rightkey=%X\n", key56, left_key, right_key);

	/* Key transformation */
	for (size_t i = 0; i < 16; i++){
		shift_key(&left_key, &right_key, i);

		const uint64_t	permuted_key_2 = (((uint64_t)left_key) << 28) | (uint64_t)right_key;
		sub_keys[i] = shift_sub_key(permuted_key_2);
//		printf("\tsub_keys[%zu] = %lX\n", i, sub_keys[i]);
	}

	for (size_t i = 0; i < 16; i++) {
		uint64_t	expanded_rpt = expand_rpt(rpt);

		// Time to XOR
		// this is the difference between encryption and decryption
		// Now the 48-bit key is XOR with 48-bit RPT and the resulting output is given to the next step
		// Which is the S-Box substitution.

		if (mode == 'd') {
			expanded_rpt ^= sub_keys[15 - i];
		} else {
			expanded_rpt ^= sub_keys[i];
		}

		uint32_t s_output = get_sbox_output(expanded_rpt);

		uint32_t	f_res = apply_post_sboxing_permuations(s_output);
		uint32_t	temp = rpt;

		rpt = lpt ^ f_res;
		lpt = temp;
	}
	uint64_t	pre_output = (((uint64_t) rpt) << 32) | (uint64_t)lpt;

	// Inverse the initial permutations
	uint64_t	output = perform_inverse_initial_permutation(pre_output);
	return (output);
}
