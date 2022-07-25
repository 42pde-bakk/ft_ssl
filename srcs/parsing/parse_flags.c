//
// Created by Peer de Bakker on 7/4/22.
//

#include <string.h>
#include "flags.h"

unsigned int parse_flags(int argc, char **argv, int *error, unsigned int *file_start_idx) {
	unsigned int flags = 0u;
	int	i;

	for (i = 2; i < argc && argv[i]; i++) {
		if (argv[i][0] != '-' || strncmp(argv[i], "--", sizeof("--")) == 0) {
			break ;
		}
		for (unsigned int i2 = 1; argv[i][i2]; i2++) {
			switch (argv[i][i2]) {
				case FLAG_APPEND_CHAR:
					flags |= FLAG_APPEND_VALUE;
					break ;
				case FLAG_QUIET_CHAR:
					flags |= FLAG_QUIET_VALUE;
					break ;
				case FLAG_REVERSE_CHAR:
					flags |= FLAG_REVERSE_VALUE;
					break ;
				case FLAG_SUM_CHAR:
					flags |= FLAG_SUM_VALUE;
					break ;
				default:
					*error = 1;
					break ;
			}
		}
	}
	*file_start_idx = i;
	return (flags);
}
