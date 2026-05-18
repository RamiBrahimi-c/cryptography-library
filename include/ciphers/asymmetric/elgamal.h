#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <gmp.h>

typedef struct {
    mpz_t p;      // large prime (> 512 bits)
    mpz_t g;      // generator
    mpz_t x;      // private key (random)
    mpz_t y;      // public key (y = g^x mod p)
    int bits;
} ElGamalKey;

typedef struct {
    mpz_t c1;     // g^k mod p
    mpz_t c2;     // M * y^k mod p
} ElGamalCiphertext;

void elgamal_generate_keypair(ElGamalKey* key, int bits);
void elgamal_encrypt(ElGamalCiphertext* ct, const mpz_t message, const ElGamalKey* key);
void elgamal_decrypt(mpz_t message, const ElGamalCiphertext* ct, const ElGamalKey* key);
void elgamal_free_key(ElGamalKey* key);
void elgamal_free_ciphertext(ElGamalCiphertext* ct);

#endif