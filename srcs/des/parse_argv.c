//
// Created by pde-bakk on 11/3/22.
//


#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "base64/flags.h"
#include "vector.h"

extern const char* g_input;
extern const char* g_output;

unsigned int parse_flags_des(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	unsigned int flags = 0;
	int opt;
	(void)string_vector;

	while ((opt = getopt(argc, argv, "+dei:o:")) != -1) {
		switch (opt) {
			case 'D':
			case 'd':
				flags |= FLAG_DECODE;
				break ;
			case 'E':
			case 'e':
				flags |= FLAG_ENCODE;
				break ;
			case 'i':
				flags |= FLAG_INPUTFILE;
				g_input = optarg;
				break ;
			case 'o':
				if (flags & FLAG_OUTPUTFILE) {
					fprintf(stderr, "You are not allowed to specify two outputfiles!\n");
					return ((unsigned int)-1);
				}
				flags |= FLAG_OUTPUTFILE;
				g_output = optarg;
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
		fprintf(stderr, "what is going on? Decode and encode!?\n");
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (flags);
}
