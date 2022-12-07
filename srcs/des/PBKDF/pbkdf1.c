//
// Created by pde-bakk on 11/23/22.
//

#include "libft.h"
#include "md5/md5.h"
#include "des/des.h"
#include <stdio.h>

void	pbkdf_1(const char *pass, uint64_t salt, const size_t iter, uint64_t* output_key, uint64_t* output_iv) {
	t_MD5Context data;
	char wowzers[17];
	const size_t len = ft_strlen(pass);

	ft_bzero(&data, sizeof(data));
	ft_bzero(wowzers, sizeof(wowzers));

	ft_memcpy(wowzers, pass, len);
	ft_memcpy(wowzers, &salt, 8);

	for (size_t i = 0; i < iter; i++) {
		data = md5sum_return_string(wowzers);
	}

	printf("digest = %016lX\n", *(uint64_t *)data.digest);

	*output_key = *(uint64_t *)data.digest;
	*output_iv = *(uint64_t *)(data.digest + 8);
}
