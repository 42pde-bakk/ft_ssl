//
// Created by pde-bakk on 11/23/22.
//

#include <stdlib.h>
#include "libft.h"
#include "md5/md5.h"

void	pbkdf_1(const char* pass, const char* salt, const size_t iter, uint64_t* output_key, uint64_t* output_iv) {
	t_MD5Context data;

	ft_bzero(&data, sizeof(data));
	char* wowzers = ft_strjoin(pass, salt);
	for (size_t i = 0; i < iter; i++) {
		data = md5sum_return_string(wowzers);
	}

	free(wowzers);
	(void)output_iv;
	(void)output_key;
}