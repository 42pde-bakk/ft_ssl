//
// Created by Peer de Bakker on 7/27/22.
//

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static size_t	count_escapable_chars(const char *haystack) {
	size_t	count = 0;
	int i = 0;

	while (haystack[i]) {
		if (haystack[i] >= '\a' && haystack[i] <= '\r') {
			// '\a', '\b', '\t', '\n', '\v', '\f', '\r'
			count++;
		}
		haystack++;
	}
	return (count);
}

char	*get_escaped_string(char *str) {
	const size_t	escapableCharsCount = count_escapable_chars(str);
	const size_t	len = strlen(str) + escapableCharsCount + 1 + 2; // +2 for " at beginning and end
	size_t	i = 0,
			i2 = 1;
	char	*buffer;

	buffer = calloc(len, sizeof(char));
	if (!buffer) {
		return (NULL);
	}
	buffer[0] = '"';
	while (str[i]) {
		if (str[i] >= '\a' && str[i] <= '\r') {
			buffer[i2] = '\\';
			switch (str[i]) {
				case '\a':
					buffer[i2 + 1] = 'a';
					break ;
				case '\b':
					buffer[i2 + 1] = 'b';
					break ;
				case '\t':
					buffer[i2 + 1] = 't';
					break ;
				case '\n':
					buffer[i2 + 1] = 'n';
					break ;
				case '\v':
					buffer[i2 + 1] = 'v';
					break ;
				case '\f':
					buffer[i2 + 1] = 'f';
					break ;
				case '\r':
					buffer[i2 + 1] = 'r';
					break ;
				default:
					break ;
			}
			i2 += 2;
			i += 1;
		} else {
			buffer[i2] = str[i];
			i2++;
			i++;
		}
	}
	buffer[i2] = '"';
	return (buffer);
}
