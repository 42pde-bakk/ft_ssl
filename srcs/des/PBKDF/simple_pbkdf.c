//
// Created by pde-bakk on 11/23/22.
//

#include <stddef.h>
#include <stdint.h>


/*
 * Stands for Password-Based Key Derivation Function
 */
uint64_t	pbkdf(uint64_t password, uint64_t salt) {
	return (password ^ salt);
}
