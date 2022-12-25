//
// Created by Peer De bakker on 7/28/22.
//

#include <stdio.h>

void print_usage(const char* prog_name) {
	const char* usage = "command [flags] [file/string]";
	fprintf(stderr, "usage: %s %s\n", prog_name, usage);
}
