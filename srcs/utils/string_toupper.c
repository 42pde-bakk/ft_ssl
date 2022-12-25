//
// Created by Peer De bakker on 7/27/22.
//

#include <stdlib.h>
#include <string.h>
#include "libft.h"

char* string_toupper(const char* const str) {
	size_t len = strlen(str);
	char* out = calloc(len + 1, sizeof(char));

	for (size_t i = 0; i < len; i++) {
		out[i] = (char) ft_toupper(str[i]);
	}
	return (out);
}
