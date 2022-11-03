//
// Created by pde-bakk on 11/3/22.
//

#include <stdint.h>
#define L64_MASK	0x00000000FFFFFFFF

void	msg_split_blocks(const uint64_t block, uint32_t* leftblock, uint32_t* rightblock) {
	*leftblock = (uint32_t)(block >> 32) & L64_MASK;
	*rightblock = (uint32_t)(block & L64_MASK);
}
