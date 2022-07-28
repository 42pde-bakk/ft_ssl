//
// Created by Peer De bakker on 7/28/22.
//

#include <stdio.h>
#include "func_ptrs.h"

extern t_handler handlers[3];


void	print_usage(const char *prog_name) {
	const char *usage = "command [flags] [file/string]";
	fprintf(stderr, "usage: %s %s\n", prog_name, usage);
}

void print_error(const char *prog_name, char *invalid_command) {
	fprintf(stderr, "%s: Error: '%s' in an invalid command.\n\n", prog_name, invalid_command);
	fprintf(stderr, "Commands:\n");
	for (size_t i = 0; handlers[i].cmd; i++) {
		fprintf(stderr, "%s\n", handlers[i].cmd);
	}
	fprintf(stderr, "\nFlags:\n-p -q -r -s\n");
}
