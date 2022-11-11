//
// Created by pde-bakk on 10/25/22.
//

#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "base64/flags.h"
#include "vector.h"

const char* g_input = NULL;
const char* g_output = NULL;
unsigned int	g_base64_flags = 0;

static void	print_usage() {
	fprintf(stderr, "base64:\nvailable flags:\n");
	fprintf(stderr, "-D or -d, decode mode\n");
	fprintf(stderr, "-E or -e, encode mode (default)\n");
	fprintf(stderr, "-i, input file\n");
	fprintf(stderr, "-o, output file\n");
	fprintf(stderr, "-h, display this summary\n");
}

unsigned int parse_flags_base64(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	int opt;
	(void)string_vector;

	while ((opt = getopt(argc, argv, "+dei:o:")) != -1) {
		switch (opt) {
			case 'D':
			case 'd':
				g_base64_flags |= FLAG_DECODE;
				break ;
			case 'E':
			case 'e':
				g_base64_flags |= FLAG_ENCODE;
				break ;
			case 'i':
				g_base64_flags |= FLAG_INPUTFILE;
				g_input = optarg;
				break ;
			case 'o':
				if (g_base64_flags & FLAG_OUTPUTFILE) {
					fprintf(stderr, "You are not allowed to specify two outputfiles!\n");
					return ((unsigned int)-1);
				}
				g_base64_flags |= FLAG_OUTPUTFILE;
				g_output = optarg;
				break ;
			case '?':
				if (optopt == 's')
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				print_usage();
				return ((unsigned int)-1);
			default:
				fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
				print_usage();
				return ((unsigned int)-1);
		}
	}
	if (g_base64_flags & FLAG_DECODE && g_base64_flags & FLAG_ENCODE) {
		fprintf(stderr, "what is going on? Decode and encode!?\n");
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (g_base64_flags);
}
