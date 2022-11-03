//
// Created by Peer de Bakker on 7/4/22.
//
#include <string.h>
#include "func_ptrs.h"
#include "parsing.h"
#include <stdio.h>

const t_handler handlers[] = {
		{
			.cmd = "md5",
			.handle_file = md5sum_file,
			.handle_string = md5sum_string,
			.handle_flags = parse_flags_md5_sha
		},
		{
			.cmd = "sha256",
			.handle_file = sha256_file,
			.handle_string = sha256_string,
			.handle_flags = parse_flags_md5_sha
		},
		{
			.cmd = "sha224",
			.handle_file = sha224_file,
			.handle_string = sha224_string,
			.handle_flags = parse_flags_md5_sha
		},
		{
			.cmd = "base64",
			.handle_file = base64_fd,
			.handle_string = base64_string,
			.handle_flags = parse_flags_base64
		},
		{
			.cmd = "des",
			.handle_file = des_fd,
			.handle_string = des_string,
			.handle_flags = parse_flags_des
		},
		{
			.cmd = NULL
		}
};

t_handler 	parse_command(const char *const cmd) {
	const size_t handler_amount = sizeof(handlers) / sizeof(t_handler);
	for (size_t i = 0; i < handler_amount; i++) {
		const char * const str = handlers[i].cmd;
		if (str && strcmp(cmd, str) == 0) {
			return (handlers[i]);
		}
	}
	return (handlers[handler_amount - 1]);
}

void print_error(const char *prog_name, char *invalid_command) {
	fprintf(stderr, "%s: Error: '%s' in an invalid command.\n\n", prog_name, invalid_command);
	fprintf(stderr, "Commands:\n");
	for (size_t i = 0; handlers[i].cmd; i++) {
		fprintf(stderr, "%s\n", handlers[i].cmd);
	}
	fprintf(stderr, "\nFlags:\n-p -q -r -s\n");
}
