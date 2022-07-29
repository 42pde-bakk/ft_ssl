//
// Created by Peer De bakker on 7/29/22.
//

#include <stdint.h>

/*
 * Rotates a 32-bit word left by n bits
 */
uint32_t leftrotate(uint32_t x, uint32_t n) {
	return (x << n) | (x >> (32 - n));
}

/*
 * Rotates a 32-bit word right by n bits
 */
uint32_t rightrotate(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}
