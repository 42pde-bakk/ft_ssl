//
// Created by pde-bakk on 11/7/22.
//
#include <stddef.h>
#include <stdint.h>

/*
 * Stands for Password-Based Key Derivation Function
 */
typedef size_t	(*f_prf)();
uint64_t	pbkdf(uint64_t password, uint64_t salt) {
	return (password ^ salt);
}
