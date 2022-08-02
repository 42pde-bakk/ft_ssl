//
// Created by Peer De bakker on 8/1/22.
//

#include <stdint.h>
#include <stdio.h>

void print_hash(const uint8_t *digest, const size_t digest_length) {
	for (size_t i = 0; i < digest_length; i++) {
		fprintf(stdout, "%02x", digest[i]);
	}
}
