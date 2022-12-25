//
// Created by Peer de Bakker on 7/4/22.
//

#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "md5/flags.h"
#include "md5/utils.h"
#include "vector.h"
#include "libft.h"

unsigned int g_flags;
int g_infd = 0;
int g_outfd = 1;


static int open_file(const char* filename) {
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

static int read_stdin(char** result_output, size_t* result_length) {
	ssize_t ret;
	char* result = calloc(1, sizeof(char));
	char* tmp = calloc(BUFSIZ + 1, sizeof(char));

	*result_length = 0;
	if (!tmp || !result) {
		free(result);
		free(tmp);
		return (EXIT_FAILURE);
	}
	while ((ret = read(STDIN_FILENO, tmp, BUFSIZ)) > 0) {
		if (ret > 0)
			*result_length += ret;
		char* tmp2 = result;
		result = ft_strjoin(result, tmp);
		free(tmp2);
		if (!result) {
			free(result);
			free(tmp);
			return (EXIT_FAILURE);
		}
		ft_bzero(tmp, BUFSIZ + 1);
	}
	free(tmp);
	if (ret < 0 || *result_length == 0) {
		if (ret < 0)
			perror("read");
		free(result);
		return (EXIT_FAILURE);
	}
	*result_output = result;
	return (EXIT_SUCCESS);
}

static int handle_stdin(t_handler handler, bool no_files_or_strings_given, const unsigned int flags) {
	ssize_t ret;
	char* result;
	size_t result_length;

	if ((!isatty(STDIN_FILENO)) && (flags & FLAG_P || no_files_or_strings_given)) {
		if (read_stdin(&result, &result_length) == EXIT_FAILURE) {
			return (EXIT_FAILURE);
		}
		char* escaped_string = get_escaped_string(result);
		if (!escaped_string) {
			free(result);
			return (EXIT_FAILURE);
		}
		if (ft_strncmp(handler.cmd, "md5", 3) == 0 || ft_strncmp(handler.cmd, "sha", 3) == 0) {
			if (!(flags & FLAG_QUIET)) {
				fprintf(stdout, "(%s)= ", flags & FLAG_P ? escaped_string : "stdin");
			} else if (flags & FLAG_P) {
				fprintf(stdout, "%s", result);
			}
		}
		ret = handler.handle_string(result, result_length);
		free(escaped_string);
		free(result);
		if (handler.print_filename)
			fprintf(stdout, "\n");
		return ((int) ret);
	}
	return (EXIT_SUCCESS);
}

static int handle_file(t_handler handler, const int fd, const char* filename, const unsigned int flags) {
	int ret;

	if (!(flags & FLAG_QUIET) && !(flags & FLAG_REVERSE) && handler.print_filename) {
		char* upper = string_toupper((char*) handler.cmd);
		fprintf(stdout, "%s (%s) = ", upper, filename);
		free(upper);
	}
	ret = handler.handle_file(fd);
	if (!(flags & FLAG_QUIET) && (flags & FLAG_REVERSE) && handler.print_filename) {
		fprintf(stdout, " %s", filename);
	}
	if (handler.print_filename)
		fprintf(stdout, "\n");
	return (ret);
}

static int handle_string(t_handler handler, char* str, const unsigned int flags) {
	int ret;
	char* escaped_string = get_escaped_string(str);

	if (!(flags & FLAG_QUIET) && !(flags & FLAG_REVERSE) && handler.print_filename) {
		char* upper = string_toupper((char*) handler.cmd);
		fprintf(stdout, "%s (%s) = ", upper, escaped_string);
		free(upper);
	}
	ret = handler.handle_string(str, ft_strlen(str));
	if (!(flags & FLAG_QUIET) && (flags & FLAG_REVERSE) && handler.print_filename) {
		fprintf(stdout, " %s", escaped_string);
	}
	free(escaped_string);
	if (handler.print_filename)
		fprintf(stdout, "\n");
	return (ret);
}

int main(int argc, char** argv) {
	unsigned int file_start_idx = 1;
	t_handler handler;
	t_ptrvector* vec;
	unsigned int ret = 0;
	const char* program_name = get_program_name(argv[0]);

	if (argc == 1) {
		// I guess I could treat it like md5sum treats no-args, but the subject has an example that wants this:
		print_usage(program_name);
		return (EXIT_FAILURE);
	}
	vec = ptrvector_init(5, false);
	if (!vec) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	handler = parse_command(argv[1]);
	if (!handler.cmd) {
		print_error(program_name, argv[1]);
		ptrvector_destroy(vec);
		return (EXIT_FAILURE);
	}
	g_flags = handler.handle_flags(argc - 1, &argv[1], &file_start_idx, vec);
	if (g_flags == (unsigned int) -1) {
		ptrvector_destroy(vec);
		return (EXIT_FAILURE);
	}

	const bool no_files_or_strings_given = (unsigned int) argc == file_start_idx && vec->size == 0;

	ret |= handle_stdin(handler, no_files_or_strings_given, g_flags);

	for (unsigned int i = 0; i < vec->size; i++) {
		ret |= handle_string(handler, (char*) vec->arr[i], g_flags);
	}
	if (g_infd > 0) {
		ret |= handle_file(handler, g_infd, "InputFile", g_flags);
		close(g_infd);
		g_infd = 0;
	}
	for (unsigned int i = file_start_idx; i < (unsigned int) argc; i++) {
		int fd;
		const char* filename = argv[i];

		fd = open_file(filename);
		if (fd == -1) {
			fprintf(stderr, "%s: %s: %s: No such file or directory\n", program_name, handler.cmd, filename);
			ret |= 1;
			continue;
		}
		ret |= handle_file(handler, fd, filename, g_flags);
		close(fd);
	}
	ptrvector_destroy(vec);
	if (g_outfd > 1) {
		close(g_outfd);
		g_outfd = 0;
	}
	if (g_infd > 0) {
		close(g_infd);
		g_infd = 0;
	}
	return ((int) ret);
}
