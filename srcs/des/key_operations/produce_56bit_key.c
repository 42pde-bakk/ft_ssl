//
// Created by pde-bakk on 11/3/22.
//

#include <stddef.h>
#include <stdint.h>

/*
 * Each index is the bit position in the plain-text block
 * Each value is which index's value will overwrite it
 */
static const unsigned int key_permutations_table[] = {
		57, 49, 41, 33, 25, 17, 9,
		1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27,
		19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15,
		7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29,
		21, 13, 5, 28, 20, 12, 4
};

uint64_t produce_56bit_key(const uint64_t key) {
	uint64_t new_key = 0;

	for (size_t i = 0; i < 56; i++) {
		new_key <<= 1;
		new_key |= (key >> (64 - key_permutations_table[i])) & 1;
	}
	return (new_key);
}
