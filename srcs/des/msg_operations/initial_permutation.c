//
// Created by pde-bakk on 11/3/22.
//

#include <stddef.h>
#include <stdint.h>

/*
 * Each index is the bit position in the plain-text block
 * Each value is which index's value will overwrite it
 */
static const int	IP_table[] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};


uint64_t	perform_initial_permutation(const uint64_t input) {
	uint64_t	initial_perm_result = 0;

	for (size_t i = 0; i < 64; i++) {
		initial_perm_result <<= 1;
		initial_perm_result |= (input >> (64 - IP_table[i])) & 1;
	}
	return (initial_perm_result);
}
