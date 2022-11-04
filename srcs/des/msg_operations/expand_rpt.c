//
// Created by pde-bakk on 11/3/22.
//

#include <stddef.h>
#include <stdint.h>

static uint32_t expansion_table[] = {
		32,  1,  2,  3,  4,  5,
		4,  5,  6,  7,  8,  9,
		8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1
};

uint64_t	expand_rpt(uint32_t rpt) {
	uint64_t expanded_rpt = 0;

	/* We expand RPT from 32 to 48 bits */
	// This is the f(R,K) function
	for (size_t j = 0; j < 48; j++) {
		expanded_rpt <<= 1;
		expanded_rpt |= (uint64_t) ((rpt >> (32 - expansion_table[j])) & 0x1);
	}
	return (expanded_rpt);
}
