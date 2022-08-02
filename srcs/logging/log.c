//
// Created by Peer De bakker on 8/2/22.
//

#include "logging.h"
#include <stdio.h>

void	write_log(const char *format, ...) {
#ifdef LOGGING
	va_list	args;

	va_start(args, format);
	vfprintf(stderr, format, args);

	va_end(args);
#else
	(void)format;
#endif
}