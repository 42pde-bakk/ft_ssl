//
// Created by pde-bakk on 11/2/22.
//

#ifndef FT_SSL_DES_H
#define FT_SSL_DES_H

#define CHUNK_SIZE 8 // Bytes, so 64 bits
#include <stdint.h>

int	des_encode_string(const char* str);
int des_encode_file(int fd);

int	des_decode_string(const char* str);
int des_decode_file(int fd);

void		msg_split_blocks(uint64_t block, uint32_t* leftblock, uint32_t* rightblock);
uint64_t	perform_initial_permutation(uint64_t input);
uint64_t	perform_inverse_initial_permutation(uint64_t input);
uint64_t	expand_rpt(uint32_t rpt);
uint32_t	get_sbox_output(const uint64_t expanded_rpt, size_t round_nb);

uint64_t	produce_56bit_key(uint64_t key);
void		key_split_blocks(uint64_t block, uint32_t* leftblock, uint32_t* rightblock);
void		shift_key(uint32_t* leftkey, uint32_t* rightkey, size_t round_nb);
uint64_t	shift_sub_key(uint64_t permuted_key);

#endif //FT_SSL_DES_H
