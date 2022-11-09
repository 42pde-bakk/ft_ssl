//
// Created by pde-bakk on 10/25/22.
//

#ifndef FT_SSL_BASE64_H
#define FT_SSL_BASE64_H

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define PAD_CHAR '='

char *base64_decode_string(const char *str, size_t str_len);
char *base64_decode_file(const int fd);
char *base64_encode_string(const char *str, size_t str_len);
char *base64_encode_file(const int fd);
extern const char*	g_input;
extern const char*	g_output;

#endif //FT_SSL_BASE64_H
