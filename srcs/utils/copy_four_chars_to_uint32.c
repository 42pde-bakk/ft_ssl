//
// Created by Peer De bakker on 7/29/22.
//
#include <stdint.h>

uint32_t	four_chars_to_uint32(const uint8_t *ptr) {
	uint32_t ret ;

	ret = ptr[0] << 24 |
		  ptr[1] << 16 |
		  ptr[2] << 8 |
		  ptr[3];

//	ret = ptr[3] << 24 |
//		  ptr[2] << 16 |
//		  ptr[1] << 8 |
//		  ptr[0];
	return (ret);
}
