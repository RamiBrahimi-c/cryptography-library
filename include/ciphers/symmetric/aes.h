#ifndef AES_H
#define AES_H

#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>
#include "aes_lib.h"

// Opaque type – actual definition in aes.c
typedef struct AesKey AesKey;

typedef struct AesKey {
    size_t   key_len;        // 16, 24, or 32
    uint8_t *key_bytes;      // raw key material
    // optionally, store an expanded AES_ctx to avoid re‑keying each time
    struct AES_ctx ctx;
};


void aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
void aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key);
void aes_set_key(void* key_struct, const char* key_str);
void aes_free_key(void* key_struct);

Cipher* get_aes_cipher(void);

#endif