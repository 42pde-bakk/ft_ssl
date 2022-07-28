#include "parsing.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "flags.h"
#include "utils.h"
#include "vector.h"

static void	print_usage(const char *prog_name) {
	const char *usage = "command [flags] [file/string]";
	fprintf(stderr, "usage: %s %s\n", prog_name, usage);
}

static int	open_file(const char *filename) {
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


static void print_hash(uint8_t *digest) {
	for (uint8_t i = 0; i < 16; i++) {
		fprintf(stdout, "%02x", digest[i]);
	}
}

int	handle_stdin(t_handler handler, const unsigned int flags) {
	int ret;
	char buf[BUFSIZ] = {0};
	char	*digest;
	if (!isatty(STDIN_FILENO) || flags & FLAG_DOUBLE_UNDERSCORE) {
		ret = (int)read(STDIN_FILENO, buf, BUFSIZ);
		if (ret <= 0) {
			perror("stdin read");
			return (EXIT_FAILURE);
		}
		if (!(flags & FLAG_QUIET)) {
			fprintf(stdout, "(%s)= ", flags & FLAG_P ? get_escaped_string(buf) : "stdin");
		}
		ret = handler.handle_string(buf, &digest);
		print_hash((uint8_t *)digest);
		free(digest);
		fprintf(stdout, "\n");
		return (ret);
	}
	return (EXIT_SUCCESS);
}

static int handle_file(t_handler handler, const int fd, const char *filename, const unsigned int flags) {
	int ret;
	char *digest;

	if (!(flags & FLAG_QUIET) && !(flags & FLAG_REVERSE)) {
		char *upper = string_toupper((char *)handler.cmd);
		fprintf(stdout, "%s (%s) = ", upper, filename);
		free(upper);
	}
	ret = handler.handle_file(fd, &digest);
	print_hash((uint8_t *)digest);
	free(digest);
	if (!(flags & FLAG_QUIET) && (flags & FLAG_REVERSE)) {
		fprintf(stdout, " %s", filename);
	}
	fprintf(stdout, "\n");
	return (ret);
}

static int handle_string(t_handler handler, char *str, const unsigned int flags) {
	int ret;
	char *digest;
	if (!(flags & FLAG_QUIET) && !(flags & FLAG_REVERSE)) {
		fprintf(stdout, "(%s)= ", flags & FLAG_P ? str : "stdin");
	}
	ret = handler.handle_string(str, &digest);
	print_hash((uint8_t *)digest);
	free(digest);
	if (!(flags & FLAG_QUIET) && (flags & FLAG_REVERSE)) {
		fprintf(stdout, " %s", str);
	}
	fprintf(stdout, "\n");
	return (ret);
}

int main(int argc, char **argv) {
	unsigned int	file_start_idx = 1;
	unsigned int	flags;
	t_handler		handler;
	t_ptrvector		*vec = ptrvector_init(5, false);
	unsigned int	ret = 0;

	if (argc == 1) {
		print_usage(argv[0]);
		return (EXIT_SUCCESS);
	}
	if (!vec) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	handler = parse_command(argv[1]);
	if (!handler.cmd) {
		fprintf(stderr, "%s:Error: '%s' is an invalid command.\n", argv[0], argv[1]);
		print_usage(argv[0]);
		return (EXIT_FAILURE);
	}
	flags = parse_flags(argc - 1, argv + 1, &file_start_idx, vec);
	if (flags == (unsigned int)-1)
		return (EXIT_FAILURE);

	ret |= handle_stdin(handler, flags);

	for (unsigned int i = 0; i < vec->size; i++) {
		fprintf(stderr, "vec->arr[%u] = %s\n", i, (char*)vec->arr[i]);
		ret |= handle_string(handler, (char *)vec->arr[i], flags);
	}

	for (unsigned int i = file_start_idx; i < (unsigned int)argc; i++) {
		int fd;
		const char *filename = argv[i];

		fd = open_file(filename);
		if (fd == -1) {
			dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", argv[0], filename);
			ret |= 1;
			continue ;
		}
		ret |= handle_file(handler, fd, filename, flags);
		close(fd);
	}
	ptrvector_destroy(vec);
	return ((int)ret);
}
