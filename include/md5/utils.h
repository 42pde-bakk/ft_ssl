//
// Created by Peer de Bakker on 7/27/22.
//

#ifndef FT_SSL_UTILS_MD5_H
#define FT_SSL_UTILS_MD5_H

#include <stdint.h>
#include <stddef.h>
#include "logging.h"

// string_toupper.c
char* string_toupper(char* str);

// escape_string.c
char* get_escaped_string(char* str);

// get_program_name.c
char* get_program_name(char* argv0);

// usage.c
void print_usage(const char* prog_name);

void print_error(const char* prog_name, char* invalid_command);

// rotations.c
uint32_t leftrotate(uint32_t x, uint32_t n);

uint32_t rightrotate(uint32_t x, uint32_t n);

// copy_four_chars_to_uint32.c
uint32_t four_chars_to_uint32_sha256(const uint8_t* ptr);

uint32_t four_chars_to_uint32_md5(const uint8_t* ptr);

// print_hash.c
void print_hash(const uint8_t* digest, size_t digest_length);

#endif //FT_SSL_UTILS_MD5_H
