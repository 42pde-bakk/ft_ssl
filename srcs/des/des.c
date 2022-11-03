//
// Created by pde-bakk on 11/2/22.
//

#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "des/flags.h"
#include "des/des.h"

int des_fd(const int fd) {
	if (g_flags & FLAG_ENCODE || 1) {
		return (des_encode_file(fd));
	} else {
		return (des_decode_file(fd));
	}
}

int des_string(const char* str) {
	if (g_flags & FLAG_DECODE) {
		return (des_decode_string(str));
	} else {
		return (des_encode_string(str));
	}
}