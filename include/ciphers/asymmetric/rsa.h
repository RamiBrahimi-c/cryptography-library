#ifndef RSA_H
#define RSA_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>
#include <gmp.h>   // GMP header

typedef struct {
    mpz_t n;      // modulus
    mpz_t e;      // public exponent
    mpz_t d;      // private exponent (only for private key)
    int   bits;   // key size (512, 1024, 2048)
    int   is_private;   // 1 if we have d, 0 if only public
} RsaKey;


void rsa_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void rsa_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void rsa_set_key(void* key_struct, const char* key_str);
void rsa_free_key(void* key_struct);

Cipher* get_des_cipher(void);

void test_rsa_32bytes(int bits) ;


#include <gmp.h>
#include <stdlib.h>
#include <time.h>



void rsa_generate_keypair(RsaKey* key, int bits, unsigned long e_value);









void rsa_export_key(const RsaKey* key);

void sayhi() ;




#endif