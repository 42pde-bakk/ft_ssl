//
// Created by Peer De bakker on 8/1/22.
//

#include "sha/utils.h"

uint32_t get_s0(const uint32_t item) {
	return (rightrotate(item, 7) ^ rightrotate(item, 18) ^ (item >> 3));
}

uint32_t get_s1(const uint32_t item) {
	return (rightrotate(item, 17) ^ rightrotate(item, 19) ^ (item >> 10));
}
