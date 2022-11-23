//
// Created by pde-bakk on 11/23/22.
//

#include "libft.h"
#include "md5/md5.h"

#define HASH_FUNCTION(x)	ft_sha256(x)
#define HASH_LEN			32

//static void	ft_hash_function(t_MD5Context data, const char *text, size_t len)
//{
//	data->text = text;
//	data->len = len;
//	HASH_FUNCTION((void *)data);
//}
//
//static char	*ft_hmac_make_key(char *key, size_t key_len)
//{
//	t_MD5Context	data;
////	char		*key64;
//	uint64_t	key64 = 0;
//
//	if (key_len <= 64)
//		ft_memcpy((void *)key64, (void *)key, key_len);
//	else
//	{
//		data = md5sum_return_string(key);
//		ft_memcpy(key64, data.digest, HASH_LEN);
//	}
//	return (key64);
//}
//
//char		*ft_hmac(char *key, char *msg, size_t key_len, size_t msg_len)
//{
//	t_md5_data	data;
//	char		*key64;
//	char		*s_i;
//	char		*s_o;
//
//	key64 = ft_hmac_make_key(key, key_len);
//	s_i = (char*)malloc(64);
//	ft_memcpy(s_i, key64, 64);
//	ft_xor_mem_value(s_i, 0x36, 64);
//	s_o = (char*)malloc(64);
//	ft_memcpy(s_o, key64, 64);
//	ft_xor_mem_value(s_o, 0x5c, 64);
//	s_i = ft_memjoin_del((void *)s_i, (void *)msg, 64, msg_len);
//	ft_hash_function(&data, s_i, msg_len + 64);
//	s_o = ft_memjoin_del((void *)s_o, (void *)data.hash, 64, HASH_LEN);
//	ft_hash_function(&data, s_o, 64 + HASH_LEN);
//	ft_memcpy((void *)key64, (void *)data.hash, HASH_LEN);
//	ft_strdel(&s_o);
//	ft_strdel(&s_i);
//	return (key64);
//}
