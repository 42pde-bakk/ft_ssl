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
#include "libft.h"

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

int	handle_stdin(t_handler handler, bool no_files_given, const unsigned int flags) {
	ssize_t	ret;
	ssize_t	total_read = 0;
	char	*digest;
	char *result = calloc(1, sizeof(char));

	if (!result) {
		return (EXIT_FAILURE);
	}
//	fprintf(stderr, "handle_stdin: %d || %d || %d\n", !isatty(STDIN_FILENO), flags & FLAG_P, argc);
	if (!isatty(STDIN_FILENO) && (flags & FLAG_P || no_files_given)) { // shouldn't check argc, but the amount of actual files/strings given
		char *tmp = calloc(BUFSIZ + 1, sizeof(char));
		if (!tmp) {
			free(result);
			return (EXIT_FAILURE);
		}
		while ((ret = read(STDIN_FILENO, tmp, BUFSIZ)) > 0) {
			if (ret > 0)
				total_read += ret;
			result = ft_strjoin(result, tmp);
			if (!result) {
				free(result);
				free(tmp);
				return (EXIT_FAILURE);
			}
			ft_bzero(tmp, BUFSIZ + 1);
		}
		if (ret < 0) {
			perror("stdin read");
			free(result);
			return (EXIT_FAILURE);
		}
		if (total_read == 0) {
			free(result);
			return (EXIT_SUCCESS);
		}
		char *escaped_string = get_escaped_string(result);
		if (!escaped_string) {
			free(result);
			return (EXIT_FAILURE);
		}
		if (!(flags & FLAG_QUIET)) {
			fprintf(stdout, "(%s)= ", flags & FLAG_P ? escaped_string : "stdin");
		} else if (flags & FLAG_P) {
			fprintf(stdout, "%s", result);
		}
		ret = handler.handle_string(result, &digest);
		print_hash((uint8_t *)digest);
		free(escaped_string);
		free(digest);
		free(result);
		fprintf(stdout, "\n");
		return ((int)ret);
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
	char *escaped_string = get_escaped_string(str);
	if (!(flags & FLAG_QUIET) && !(flags & FLAG_REVERSE)) {
		char *upper = string_toupper((char *)handler.cmd);
		fprintf(stdout, "%s (%s) = ", upper, escaped_string);
		free(upper);
	}
	ret = handler.handle_string(str, &digest);
	print_hash((uint8_t *)digest);
	free(digest);
	if (!(flags & FLAG_QUIET) && (flags & FLAG_REVERSE)) {
		fprintf(stdout, " %s", escaped_string);
	}
	fprintf(stdout, "\n");
	free(escaped_string);
	return (ret);
}

int main(int argc, char **argv) {
	unsigned int	file_start_idx = 1;
	unsigned int	flags;
	t_handler		handler;
	t_ptrvector		*vec = ptrvector_init(5, false);
	unsigned int	ret = 0;
	const char		*program_name = get_program_name(argv[0]);

	if (argc == 1) {
		print_usage(program_name);
		return (EXIT_SUCCESS);
	}
	if (!vec) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	handler = parse_command(argv[1]);
	if (!handler.cmd) {
		print_error(program_name, argv[1]);
		return (EXIT_FAILURE);
	}
	flags = parse_flags(argc - 1, argv + 1, &file_start_idx, vec);
	if (flags == (unsigned int)-1)
		return (EXIT_FAILURE);

	ret |= handle_stdin(handler, (unsigned int)argc == file_start_idx, flags);

	for (unsigned int i = 0; i < vec->size; i++) {
		ret |= handle_string(handler, (char *)vec->arr[i], flags);
	}

	for (unsigned int i = file_start_idx; i < (unsigned int)argc; i++) {
		int fd;
		const char *filename = argv[i];

		fd = open_file(filename);
		if (fd == -1) {
			fprintf(stderr, "%s: %s: %s: No such file or directory\n", program_name, handler.cmd, filename);
			ret |= 1;
			continue ;
		}
		ret |= handle_file(handler, fd, filename, flags);
		close(fd);
	}
	ptrvector_destroy(vec);
	return ((int)ret);
}
