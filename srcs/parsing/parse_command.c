//
// Created by Peer de Bakker on 7/4/22.
//
#include <string.h>
#include <stdio.h>
#include "func_ptrs.h"

static const t_handler handlers[] = {
		{
			.cmd = "md5",
			.handle_file = md5sum_file,
			.handle_string = md5sum_string
		},
		{
			.cmd = "sha256",
			.handle_file = sha256_file,
			.handle_string = sha256_string
		},
		{
			.cmd = NULL
		}
};

t_handler 	parse_command(const char *const cmd) {
	const size_t handler_amount = sizeof(handlers) / sizeof(t_handler);
//	fprintf(stderr, "handler_amount = %zu\n", handler_amount);
	for (size_t i = 0; i < handler_amount; i++) {
		const char * const str = handlers[i].cmd;
		if (str && strncmp(cmd, str, strlen(str)) == 0) {
			return (handlers[i]);
		}
	}
	return (handlers[handler_amount - 1]);
}
