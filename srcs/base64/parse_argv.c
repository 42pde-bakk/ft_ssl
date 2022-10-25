//
// Created by pde-bakk on 10/25/22.
//

#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "base64/flags.h"
#include "vector.h"

unsigned int parse_flags_base64(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	unsigned int flags = 0;
	int opt;

	while ((opt = getopt(argc, argv, "+pqrs:")) != -1) {
		switch (opt) {
			case 'd':
				flags |= FLAG_DECODE;
				break ;
			case 'e':
				flags |= FLAG_ENCODE;
				break ;
			case 'i':
				flags |= FLAG_INPUTFILE;
				if (string_vector) {
					ptrvector_pushback(string_vector, optarg);
				}
				break ;
			case 'o':
				flags |= FLAG_OUTPUTFILE;
				if (string_vector) {
					ptrvector_pushback(string_vector, optarg);
				}
				break ;
			case '?':
				if (optopt == 's')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return ((unsigned int)-1);
			default:
				fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
				return ((unsigned int)-1);
		}
	}
	if (flags & FLAG_DECODE && flags & FLAG_ENCODE) {
		dprintf(2, "what is going on? Decode and encode!?\n");
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (flags);
}