//
// Created by pde-bakk on 11/3/22.
//

#include <stddef.h>
#include <stdint.h>

/*
 * Each index is the bit position in the plain-text block
 * Each value is which index's value will overwrite it
 */
static const unsigned int Inverse_IP_table[] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9, 49, 17, 57, 25
};


uint64_t perform_inverse_initial_permutation(const uint64_t input) {
	uint64_t inverse_perm_result = 0;

	for (size_t i = 0; i < 64; i++) {
		inverse_perm_result <<= 1;
		inverse_perm_result |= (input >> (64 - Inverse_IP_table[i])) & 1;
	}
	return (inverse_perm_result);
}
