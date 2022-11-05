//
// Created by pde-bakk on 11/2/22.
//

#ifndef FT_SSL_FLAGS_H
#define FT_SSL_FLAGS_H

#define FLAG_DECODE		(1 << 0)
#define FLAG_ENCODE		(1 << 1)
#define FLAG_BASE64		(1 << 2)
#define FLAG_INPUTFILE	(1 << 3)
#define FLAG_KEY		(1 << 4)
#define FLAG_OUTPUTFILE	(1 << 5)
#define FLAG_PASSWORD	(1 << 6)
#define FLAG_SALT		(1 << 7)
#define FLAG_INITVECTOR	(1 << 8)

extern unsigned int g_flags;
extern const char*	g_infile;
extern const char*	g_outfile;
extern const char*	g_key;
extern const char*	g_password;
extern const char*	g_salt;
extern const char*	g_initialization_vector;

#endif //FT_SSL_FLAGS_H
