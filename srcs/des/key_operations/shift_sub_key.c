//
// Created by pde-bakk on 11/3/22.
//

#include <stdint.h>
#include <stddef.h>

/* Permuted Choice 2 Table */
static const unsigned int PC2[] = {
		14, 17, 11, 24,  1,  5,
		3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8,
		16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32
};

uint64_t	shift_sub_key(uint64_t permuted_key) {
	uint64_t	new_subkey = 0;
	for (size_t j = 0; j < 48; j++) {
		new_subkey <<= 1;
		new_subkey |= (permuted_key >> (56 - PC2[j])) & 0x1;
	}
	return (new_subkey);
}
