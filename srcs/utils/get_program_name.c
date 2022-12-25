//
// Created by Peer De bakker on 7/28/22.
//

#include <string.h>

char* get_program_name(char* argv0) {
	if (strncmp(argv0, "./", 2) == 0) {
		return (argv0 + 2);
	}
	return (argv0);
}
