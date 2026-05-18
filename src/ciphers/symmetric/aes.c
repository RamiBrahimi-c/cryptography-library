#ifndef AES_C
#define AES_C

#define AES128 1   // or AES192 or AES256
#define CBC 1
#define ECB 0
#define CTR 0


#include "aes.h"
#include "aes_lib.h"   // the tiny-AES library
#include <stdlib.h>
#include <string.h>
#include <stdio.h>     




void aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const AesKey* aes = (const AesKey*)key;
    if (!aes || !input || !output || length <= 0) return;

    // 1. Calculate padded length (next multiple of AES_BLOCKLEN)
    size_t block = AES_BLOCKLEN;   // 16
    size_t pad = block - (length % block);
    size_t padded_len = length + pad;

    // 2. Create temporary buffer for plaintext + padding
    uchar_t* temp = (uchar_t*)malloc(padded_len);
    if (!temp) return;
    memcpy(temp, input, length);
    memset(temp + length, (uchar_t)pad, pad);   // PKCS#7

    // 3. Encrypt in CBC mode (in‑place)
    struct AES_ctx ctx = aes->ctx;   // copy, because CBC updates the IV
    AES_CBC_encrypt_buffer(&ctx, temp, padded_len);

    // 4. Copy result to output
    memcpy(output, temp, padded_len);
    free(temp);
}



void aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const AesKey* aes = (const AesKey*)key;
    if (!aes || !input || !output || length <= 0 || length % AES_BLOCKLEN != 0)
        return;

    // 1. Decrypt the whole buffer (must be multiple of block size)
    uchar_t* temp = (uchar_t*)malloc(length);
    if (!temp) return;
    memcpy(temp, input, length);

    struct AES_ctx ctx = aes->ctx;
    AES_CBC_decrypt_buffer(&ctx, temp, length);

    // 2. Remove PKCS#7 padding
    uchar_t pad = temp[length - 1];
    if (pad == 0 || pad > AES_BLOCKLEN) {
        free(temp);   // invalid padding
        return;
    }

    // Optional: verify all padding bytes
    for (int i = length - pad; i < length; i++) {
        if (temp[i] != pad) {
            free(temp);
            return;
        }
    }

    size_t unpadded_len = length - pad;
    memcpy(output, temp, unpadded_len);
    free(temp);
}


#include <assert.h>

void aes_set_key(void* key_struct, const char* key_str)
{
    assert(key_struct != NULL  && "key struct is null lilbro") ; 
    AesKey* key = (AesKey*)key_struct;
    printf("hiii\n");
    key->key_bytes = malloc(16) ;
    key->key_len = 16 ;  
    memcpy(key->key_bytes, key_str, AES_KEYLEN);
    printf("hiii\n");
    // Also initialise the AES context (key expansion + default IV)
    static const uint8_t default_iv[16] = {0};   // all‑zero IV (INSECURE for real use)
    AES_init_ctx_iv(&key->ctx, key->key_bytes, default_iv);
}



void aes_free_key(void* key_struct)
{
    // same convention: caller does aes_free_key(&key);
    AesKey **ppKey = (AesKey**)key_struct;
    if (ppKey && *ppKey) {
        free((*ppKey)->key_bytes);
        free(*ppKey);
        *ppKey = NULL;
    }
}







#endif