//
// Created by pde-bakk on 11/2/22.
//

#ifndef FT_SSL_DES_H
#define FT_SSL_DES_H

#define CHUNK_SIZE_IN_BYTES 8 // Bytes, so 64 bits
#include <stdint.h>

/*
 * srcs/des/des.c
 */
uint64_t	apply_des(uint64_t input, uint64_t key);

/*
 * srcs/des/msg_operations
 */
void		msg_split_blocks(uint64_t block, uint32_t* leftblock, uint32_t* rightblock);
uint64_t	perform_initial_permutation(uint64_t input);
uint64_t	perform_inverse_initial_permutation(uint64_t input);
uint64_t	expand_rpt(uint32_t rpt);
uint32_t	get_sbox_output(uint64_t expanded_rpt);
uint64_t	apply_post_sboxing_permuations(uint32_t s_output);

/*
 * srcs/des/key_operations
 */
uint64_t	produce_56bit_key(uint64_t key);
void		key_split_blocks(uint64_t block, uint32_t* leftblock, uint32_t* rightblock);
void		shift_key(uint32_t* leftkey, uint32_t* rightkey, size_t round_nb);
uint64_t	shift_sub_key(uint64_t permuted_key);

/*
 * srcs/des/pbkdf.c
 */
uint64_t	pbkdf(uint64_t password, uint64_t salt);

/*
 * srcs/des/parsing.c
 */
uint64_t	create_64bit_chunk_from_str(const char* str);
uint64_t	create_64bit_chunk_from_hexstr(const char* str);
uint64_t	get_key();

/*
 * srcs/des/output.c
 */
void	output_chunk(int fd, uint64_t chunk);

#endif //FT_SSL_DES_H
