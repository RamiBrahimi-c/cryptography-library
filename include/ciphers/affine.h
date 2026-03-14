#ifndef AFFINE_H
#define AFFINE_H

#include "cipher_interface.h"
#include "../common/constants.h"
#include <stdint.h>

typedef struct {
    uint64_t a;
    uint64_t b;
} AffineKey;



void affine_encrypt(const uchar_t* input, uchar_t* output, const void* key);
void affine_decrypt(const uchar_t* input, uchar_t* output, const void* key);
void affine_set_key(void* key_struct, const char* key_str);
void affine_free_key(void* key_struct);

Cipher* get_affine_cipher(void);

#endif