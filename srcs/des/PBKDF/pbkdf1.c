//
// Created by pde-bakk on 11/23/22.
//

#include "libft.h"
#include "md5/md5.h"
#include <stdio.h>

void	pbkdf_1(uint64_t pass, uint64_t salt, const size_t iter, uint64_t* output_key, uint64_t* output_iv) {
	t_MD5Context data;
	char wowzers[17];

	printf("pass = %016lX\n", pass);
	printf("salt = %016lX\n", salt);

	ft_bzero(&data, sizeof(data));
	ft_bzero(wowzers, sizeof(wowzers));
	ft_strlcpy(wowzers, (char *)&pass, 8);
	ft_strlcpy(wowzers + 8, (char *)&salt, 8);

	for (size_t i = 0; i < iter; i++) {
		data = md5sum_return_string(wowzers);
	}

	(void)output_iv;
	*output_key = *(uint64_t *)data.digest;
	*output_iv = *(uint64_t *)(data.digest + 8);
}
