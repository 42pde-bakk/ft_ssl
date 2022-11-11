//
// Created by pde-bakk on 11/3/22.
//


#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "des/flags.h"
#include "vector.h"
#include <fcntl.h>

const char*	g_infile = NULL;
const char*	g_outfile = NULL;
int			g_outfd = 1;
const char*	g_key = NULL;
const char*	g_password = NULL;
const char*	g_salt = NULL;
const char*	g_initialization_vector = NULL;
unsigned int	g_des_flags = 0;

static int	create_outfd(const char* const pathname) {
	int fd;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
		return (1);
	g_outfd = fd;
	return (0);
}

static void	print_usage() {
	fprintf(stderr, "Data Encryption Standard:\nvailable flags:\n");
	fprintf(stderr, "-a, decode/encode the input/output in base64, depending on the encryption mode\n");
	fprintf(stderr, "-D or -d, decrypt mode\n");
	fprintf(stderr, "-E or -e, encrypt mode (default)\n");
	fprintf(stderr, "-i, input file for message\n");
	fprintf(stderr, "-o, output file for message\n");
	fprintf(stderr, "-K or -k, key in hex is the next argument\n");
	fprintf(stderr, "-p, the password in ascii is the next argument\n");
	fprintf(stderr, "-s, the salt in hex is the next argument\n");
	fprintf(stderr, "-v, the initialization vector in hex is the next argument\n");
	fprintf(stderr, "-P, print the iv/key\n");
	fprintf(stderr, "-n, disable the size difference byte padding scheme\n");
	fprintf(stderr, "-h, display this summary\n");
}

unsigned int parse_flags_des(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	int opt;
	(void)string_vector;

	while ((opt = getopt(argc, argv, "+aDdhnEePi:k:K:o:p:s:v:")) != -1) {
		switch (opt) {
			case 'a':
				g_des_flags |= FLAG_BASE64;
				break ;
			case 'D':
			case 'd':
				g_des_flags |= FLAG_DECODE;
				break ;
			case 'E':
			case 'e':
				g_des_flags |= FLAG_ENCODE;
				break ;
			case 'i':
				g_des_flags |= FLAG_INPUTFILE;
				g_infile = optarg;
				ptrvector_pushback(string_vector, (void *)g_infile);
				break ;
			case 'K':
			case 'k':
				g_des_flags |= FLAG_KEY;
				g_key = optarg;
				break ;
			case 'o':
				if (g_des_flags & FLAG_OUTPUTFILE) {
					fprintf(stderr, "You are not allowed to specify two outputfiles!\n");
					return ((unsigned int)-1);
				}
				g_des_flags |= FLAG_OUTPUTFILE;
				g_outfile = optarg;
				if (create_outfd(g_outfile)) {
					fprintf(stderr, "Error opening outfile '%s'.\n", g_outfile);
					return ((unsigned int)-1);
				}
				break ;
			case 'p':
				g_des_flags |= FLAG_PASSWORD;
				g_password = optarg;
				break ;
			case 'P':
				g_des_flags |= FLAG_SHOW_KEY;
				break ;
			case 's':
				g_des_flags |= FLAG_SALT;
				g_salt = optarg;
				break ;
			case 'v':
				g_des_flags |= FLAG_INITVECTOR;
				g_initialization_vector = optarg;
				break ;
			case 'n':
				g_des_flags |= FLAG_NO_PADDING;
				break ;
			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
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
	if (!(g_des_flags & FLAG_DECODE))
		g_des_flags |= FLAG_ENCODE;
	if (g_des_flags & FLAG_DECODE && g_des_flags & FLAG_ENCODE) {
		fprintf(stderr, "what is going on? Decode and encode!?\n");
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (g_des_flags);
}
