//
// Created by pde-bakk on 11/4/22.
//

#include <stddef.h>
#include <stdint.h>

/*
 * Each index is the bit position in the plain-text block
 * Each value is which index's value will overwrite it
 */
static const uint32_t	permutations[] = {
		16,  7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25
};

uint64_t	apply_post_sboxing_permuations(const uint32_t s_output) {
	uint32_t	f_res = 0;

	for (size_t j = 0; j < 32; j++) {
		f_res <<= 1;
		f_res |= (s_output >> (32 - permutations[j])) & 0x1;
	}
	return (f_res);
}
