#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void	print_usage(const char *prog_name) {
	const char *usage = "command [flags] [file/string]";
	fprintf(stderr, "usage: %s %s\n", prog_name, usage);
}

int	handle_file(const char *filename) {
	struct stat buf;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		return (-1);
	}
	if (fstat(fd, &buf) == -1 || buf.st_size < 0) {
		return (-1);
	}
	return (fd);
}

int main(int argc, char **argv) {
	unsigned int	file_start_idx = 1;
	unsigned int	flags;
	int				error;
	cmd_func_t		cmd_func;
	unsigned int	ret = 0;

	if (argc == 1) {
		print_usage(argv[0]);
		return (EXIT_SUCCESS);
	}
	cmd_func = parse_command(argv[1]);
	if (!cmd_func) {
		fprintf(stderr, "%s:Error: '%s' is an invalid command.\n", argv[0], argv[1]);
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	flags = parse_flags(argc, argv, &error, &file_start_idx);

	for (unsigned int i = file_start_idx; i < (unsigned int)argc; i++) {
		int fd = handle_file(argv[i]);
		if (fd == -1) {
			dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", argv[0], argv[i]);
			ret |= 1;
			continue ;
		}
		ret |= cmd_func(flags, fd);
		close(fd);
		printf("  %s\n", argv[i]);
	}

	return (ret);
}
