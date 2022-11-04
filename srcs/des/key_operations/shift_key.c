//
// Created by pde-bakk on 11/3/22.
//

#include <stdint.h>
#include <stddef.h>
#define LOWERMASK 0x0fffffff
#define UPPERMASK 0x1

static const uint32_t shift_amounts[] = {
		1,  1,  2,  2,  2,  2,  2,  2,
		1,  2,  2,  2,  2,  2,  2,  1
};

void	shift_key(uint32_t* leftkey, uint32_t* rightkey, size_t round_nb) {
	for (size_t j = 0; j < shift_amounts[round_nb]; j++) {
		*leftkey = (LOWERMASK & (*leftkey << 1)) | ((*leftkey >> 27) & 1);
		*rightkey = (LOWERMASK & (*rightkey << 1)) | ((*rightkey >> 27) & 1);
	}
}
