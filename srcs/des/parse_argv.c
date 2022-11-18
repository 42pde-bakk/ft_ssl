//
// Created by pde-bakk on 11/3/22.
//


#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include "des/flags.h"
#include "vector.h"
#include <fcntl.h>

const char*	g_key = NULL;
const char*	g_password = NULL;
const char*	g_salt = NULL;
const char*	g_initialization_vector = NULL;
unsigned int	g_des_flags = 0;

static int	create_fd(const char* const pathname, int* fd_store, int open_flag) {
	int fd;

	fd = open(pathname, open_flag, S_IRWXU);
	if (fd == -1)
		return (1);
	*fd_store = fd;
	return (0);
}

static void	print_usage() {
	fprintf(stderr, "Data Encryption Standard:\nAvailable flags:\n");
	fprintf(stderr, "\t-a, decode/encode the input/output in base64, depending on the encryption mode\n");
	fprintf(stderr, "\t-D or -d, decrypt mode\n");
	fprintf(stderr, "\t-E or -e, encrypt mode (default)\n");
	fprintf(stderr, "\t-i, input file for message\n");
	fprintf(stderr, "\t-o, output file for message\n");
	fprintf(stderr, "\t-K or -k, key in hex is the next argument\n");
	fprintf(stderr, "\t-p, the password in ascii is the next argument\n");
	fprintf(stderr, "\t-s, the salt in hex is the next argument\n");
	fprintf(stderr, "\t-v, the initialization vector in hex is the next argument\n");
	fprintf(stderr, "\t-P, print the iv/key\n");
	fprintf(stderr, "\t-n, disable the size difference byte padding scheme\n");
	fprintf(stderr, "\t-h, display this summary\n");
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
				g_des_flags |= FLAG_DECRYPT;
				break ;
			case 'E':
			case 'e':
				g_des_flags |= FLAG_ENCRYPT;
				break ;
			case 'i':
				g_des_flags |= FLAG_INPUTFILE;
				if (create_fd(optarg, &g_infd, O_RDONLY)) {
					fprintf(stderr, "Error opening infile '%s'.\n", optarg);
					return ((unsigned int)-1);
				}
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
				if (create_fd(optarg, &g_outfd, O_WRONLY | O_CREAT | O_TRUNC)) {
					fprintf(stderr, "Error opening outfile '%s'.\n", optarg);
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
	if (!(g_des_flags & FLAG_DECRYPT))
		g_des_flags |= FLAG_ENCRYPT;
	if (g_des_flags & FLAG_DECRYPT && g_des_flags & FLAG_ENCRYPT) {
		fprintf(stderr, "what is going on? Decode and encode!?\n");
	}
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (g_des_flags);
}
