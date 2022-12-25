//
// Created by pde-bakk on 10/25/22.
//

#ifndef FT_SSL_FLAGS_H
#define FT_SSL_FLAGS_H

// Base64 flags
#define FLAG_DECRYPT                0b1000
#define FLAG_ENCRYPT                0b0100
#define FLAG_INPUTFILE            0b0010
#define FLAG_OUTPUTFILE            0b0001

extern unsigned int g_base64_flags;
extern int g_infd;
extern int g_outfd;


#endif //FT_SSL_FLAGS_H
