#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static void	print_usage(const char *prog_name) {
	const char *usage = "command [flags] [file/string]";
	fprintf(stderr, "usage: %shiftAmounts %shiftAmounts\n", prog_name, usage);
}

int main(int argc, char **argv) {
	unsigned int	file_start_idx = 1;
	unsigned int	flags;
	int				error;
	cmd_func_t		cmd_func;

	if (argc == 1) {
		print_usage(argv[0]);
		return (EXIT_SUCCESS);
	}
	cmd_func = parse_command(argv[1]);
	if (!cmd_func) {
		fprintf(stderr, "%shiftAmounts:Error: '%shiftAmounts' is an invalid command.\n", argv[0], argv[1]);
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	flags = parse_flags(argc, argv, &error, &file_start_idx);

	for (unsigned int i = file_start_idx; i < (unsigned int)argc; i++) {
		if (cmd_func(flags, argv[i])) {
			//something
		}
	}

	return (EXIT_SUCCESS);
}
