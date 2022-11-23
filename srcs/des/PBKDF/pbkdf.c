//
// Created by pde-bakk on 11/7/22.
//
#include <stddef.h>
#include <stdint.h>
#include "libft.h"
#define HASH_LEN			32

//static char	*ft_pbkdf2_f(char *pass, uint8_t *salt, size_t iter, int num)
//{
//	char	int_msb[12];
//	char	*block;
//	char	*next_block;
//	char	result_block[HASH_LEN];
//	size_t	pass_len;
//
//	ft_memcpy((void *)int_msb, (void *)salt, 8);
//	ft_memcpy(int_msb + 8, (void *)&num, 4);
//	ft_memrev(int_msb + 8, 4);
//	pass_len = ft_strlen(pass);
//	block = ft_hmac(pass, int_msb, pass_len, 12);
//	if (iter == 1)
//		return (block);
//	ft_memcpy(result_block, block, HASH_LEN);
//	while (--iter)
//	{
//		next_block = ft_hmac(pass, block, pass_len, HASH_LEN);
//		ft_strdel(&block);
//		block = next_block;
//		ft_xor_mem(result_block, block, HASH_LEN);
//	}
//	ft_memcpy(block, result_block, HASH_LEN);
//	return (block);
//}
//
//char		*ft_pbkdf2(char *pass, uint8_t *salt, size_t iter, size_t len)
//{
//	char	*key;
//	char	*key_next;
//	size_t	block_counter;
//	size_t	blocks;
//	size_t	current_len;
//
//	blocks = (len + HASH_LEN - 1) / HASH_LEN;
//	key = ft_pbkdf2_f(pass, salt, iter, 1);
//	current_len = HASH_LEN;
//	block_counter = 1;
//	while (++block_counter <= blocks)
//	{
//		key_next = ft_pbkdf2_f(pass, salt, iter, block_counter);
//		key = ft_memjoin_del(key, key_next, current_len, HASH_LEN);
//		current_len += HASH_LEN;
//		ft_strdel(&key_next);
//	}
//	return (key);
//}