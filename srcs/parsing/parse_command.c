//
// Created by Peer de Bakker on 7/4/22.
//
#include <string.h>
#include "func_ptrs.h"

cmd_func_t	parse_command(const char *const cmd) {
	if (strncmp(cmd, "md5", strlen("md5sum")) == 0) {
		return (md5sum_file);
	}
	if (strncmp(cmd, "sha256", strlen("sha256")) == 0) {
		return (sha256);
	}
	return (NULL);
}
