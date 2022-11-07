//
// Created by pde-bakk on 11/7/22.
//
#include <stddef.h>
#include <stdint.h>
/*
 * Stands for Password-Based Key Derivation Function
 */
typedef size_t	(*f_prf)();
uint64_t	pbkdf(f_prf prf, uint64_t password, uint64_t salt, uint32_t iterations, uint32_t bitlen) {

}