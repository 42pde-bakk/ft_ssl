//
// Created by Peer de Bakker on 7/4/22.
//

#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "md5/flags.h"
#include "vector.h"

unsigned int parse_flags_md5_sha(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	unsigned int flags = 0;
	int opt;

	while ((opt = getopt(argc, argv, "+pqrs:")) != -1) {
		switch (opt) {
			case 'p':
				flags |= FLAG_P;
				break;
			case 'q':
				flags |= FLAG_QUIET;
				break;
			case 'r':
				flags |= FLAG_REVERSE;
				break;
			case 's':
				flags |= FLAG_STRING;
				if (string_vector) {
					ptrvector_pushback(string_vector, optarg);
				}
				break;
			case '?':
				if (optopt == 's')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return ((unsigned int) -1);
			default:
				fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
				return ((unsigned int) -1);
		}
	}
	if (flags & FLAG_QUIET) {
		flags &= ~FLAG_REVERSE;
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (flags);
}
