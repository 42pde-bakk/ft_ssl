//
// Created by pde-bakk on 11/2/22.
//

#ifndef FT_SSL_DES_H
#define FT_SSL_DES_H

#define CHUNK_SIZE_IN_BYTES 8 // Bytes, so 64 bits
#define ROUNDS 16
#define PBKDF_VERSION 2
#include <stdint.h>
#include <stdbool.h>

/*
 * srcs/des/des.c
 */
uint64_t apply_des(uint64_t input, uint64_t key, int mode);

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
void	pbkdf_1(const char *pass, uint64_t salt, size_t iter, uint64_t* output_key, uint64_t* output_iv);
char * hmac(char *key, const char *msg);
int
pbkdf_2(char *pass, uint64_t salt, size_t iter, uint64_t *key, uint64_t *iv);
/*
 * srcs/des/parsing.c
 */
uint64_t	create_64bit_chunk_from_str(const char* str);
uint64_t	create_64bit_chunk_from_hexstr(const char* str);
uint64_t	get_key();

/*
 * srcs/des/output.c
 */
uint64_t	REV64(uint64_t x);
uint64_t	REV32(uint32_t x);
void	add_chunk_to_buffer(uint64_t chunk, bool should_reverse);
void	clear_buffer(int fd, bool reverse_decode);
void create_str_from_64bit_chunk_and_output(uint64_t chunk, int fd, size_t write_len);

#endif //FT_SSL_DES_H
