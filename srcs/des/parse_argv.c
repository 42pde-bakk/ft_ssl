//
// Created by pde-bakk on 11/3/22.
//


#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "des/flags.h"
#include "vector.h"

const char*	g_infile = NULL;
const char*	g_outfile = NULL;
const char*	g_key = NULL;
const char*	g_password = NULL;
const char*	g_salt = NULL;
const char*	g_initialization_vector = NULL;

unsigned int parse_flags_des(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	unsigned int flags = 0;
	int opt;
	(void)string_vector;

	while ((opt = getopt(argc, argv, "+aDdEePi:k:o:p:s:v:")) != -1) {
		switch (opt) {
			case 'a':
				flags |= FLAG_BASE64;
				break ;
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
				g_infile = optarg;
				break ;
			case 'k':
				flags |= FLAG_KEY;
				g_key = optarg;
				break ;
			case 'o':
				if (flags & FLAG_OUTPUTFILE) {
					fprintf(stderr, "You are not allowed to specify two outputfiles!\n");
					return ((unsigned int)-1);
				}
				flags |= FLAG_OUTPUTFILE;
				g_outfile = optarg;
				break ;
			case 'p':
				flags |= FLAG_PASSWORD;
				g_password = optarg;
				break ;
			case 'P':
				flags |= FLAG_SHOW_KEY;
				break ;
			case 's':
				flags |= FLAG_SALT;
				g_salt = optarg;
				break ;
			case 'v':
				flags |= FLAG_INITVECTOR;
				g_initialization_vector = optarg;
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
