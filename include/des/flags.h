//
// Created by pde-bakk on 11/2/22.
//

#ifndef FT_SSL_FLAGS_H
#define FT_SSL_FLAGS_H

#define FLAG_DECRYPT    (1 << 0)
#define FLAG_ENCRYPT    (1 << 1)
#define FLAG_BASE64        (1 << 2)
#define FLAG_INPUTFILE    (1 << 3)
#define FLAG_KEY        (1 << 4)
#define FLAG_OUTPUTFILE    (1 << 5)
#define FLAG_PASSWORD    (1 << 6)
#define FLAG_SALT        (1 << 7)
#define FLAG_INITVECTOR    (1 << 8)
#define FLAG_SHOW_KEY    (1 << 9)
#define FLAG_NO_PADDING    (1 << 10)


extern unsigned int g_des_flags;
extern const char* g_infile;
extern const char* g_outfile;
extern const char* g_key;
extern const char* g_password;
extern const char* g_salt;
extern const char* g_initialization_vector;
extern int pbkdf_version;
extern int g_infd;
extern int g_outfd;

#endif //FT_SSL_FLAGS_H
