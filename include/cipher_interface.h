#ifndef CIPHER_INTERFACE_H
#define CIPHER_INTERFACE_H

#include "./common/constants.h"
#include <stddef.h>

typedef struct {
    void (*encrypt)(const uchar_t* input, uchar_t* output, const void* key);
    void (*decrypt)(const uchar_t* input, uchar_t* output, const void* key);
    void (*set_key)(void* key_struct, const char* key_str);
    void (*free_key)(void* key_struct);
    char* name;
} Cipher;




void register_cipher(Cipher* cipher);
Cipher* get_cipher(const char* name);

#endif