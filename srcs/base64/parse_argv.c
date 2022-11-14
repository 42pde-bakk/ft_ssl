//
// Created by pde-bakk on 10/25/22.
//

#include <getopt.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include "base64/flags.h"
#include "vector.h"
unsigned int	g_base64_flags = 0;

static int	create_fd(const char* const pathname, int* fd_store, int open_flag) {
	int fd;

	fd = open(pathname, open_flag, S_IRWXU);
	if (fd == -1)
		return (1);
	*fd_store = fd;
	return (0);
}

static void	print_usage() {
	fprintf(stderr, "base64:\nvAailable flags:\n");
	fprintf(stderr, "\t-D or -d, decode mode\n");
	fprintf(stderr, "\t-E or -e, encode mode (default)\n");
	fprintf(stderr, "-\ti, input file\n");
	fprintf(stderr, "\t-o, output file\n");
	fprintf(stderr, "\t-h, display this summary\n");
}

unsigned int parse_flags_base64(int argc, char** argv, unsigned int* file_start_idx, t_ptrvector* string_vector) {
	int opt;
	(void)string_vector;

	while ((opt = getopt(argc, argv, "+dei:o:")) != -1) {
		switch (opt) {
			case 'D':
			case 'd':
				g_base64_flags |= FLAG_DECRYPT;
				break ;
			case 'E':
			case 'e':
				g_base64_flags |= FLAG_ENCRYPT;
				break ;
			case 'i':
				g_base64_flags |= FLAG_INPUTFILE;
				if (create_fd(optarg, &g_infd, O_RDONLY)) {
					fprintf(stderr, "Error opening %s\n", optarg);
					exit(EXIT_FAILURE);
				}
				break ;
			case 'o':
				if (g_base64_flags & FLAG_OUTPUTFILE) {
					fprintf(stderr, "You are not allowed to specify two outputfiles!\n");
					return ((unsigned int)-1);
				}
				g_base64_flags |= FLAG_OUTPUTFILE;
				if (create_fd(optarg, &g_outfd, O_WRONLY | O_TRUNC | O_CREAT)) {
					fprintf(stderr, "Error opening %s\n", optarg);
					exit(EXIT_FAILURE);
				}
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
	if (g_base64_flags & FLAG_DECRYPT && g_base64_flags & FLAG_ENCRYPT) {
		fprintf(stderr, "what is going on? Decode and encode!?\n");
	}
	if (!(g_base64_flags & FLAG_DECRYPT))
		g_base64_flags |= FLAG_ENCRYPT;
	if (file_start_idx) {
		*file_start_idx = optind + 1;
	}
	return (g_base64_flags);
}
