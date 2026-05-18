#include "elgamal.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void elgamal_generate_keypair(ElGamalKey* key, int bits)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    unsigned long seed = time(NULL);
    if (seed == 0) seed = 42;
    gmp_randseed_ui(state, seed);

    mpz_inits(key->p, key->g, key->x, key->y, NULL);

    // 1. Generate large prime p (> 512 bits)
    mpz_urandomb(key->p, state, bits);
    mpz_setbit(key->p, bits - 1);   // ensure exactly 'bits' bits
    mpz_nextprime(key->p, key->p);

    // 2. Choose generator g (small prime like 2 or 5 works if safe prime, else random)
    // For simplicity, use g = 2 (check if it's a generator, else 3, 5...)
    unsigned long small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    mpz_t p_minus_1, q, test;
    mpz_inits(p_minus_1, q, test, NULL);
    mpz_sub_ui(p_minus_1, key->p, 1);
    
    int found = 0;
    for (int i = 0; i < 10; i++) {
        mpz_set_ui(key->g, small_primes[i]);
        // Quick check: g^((p-1)/2) mod p != 1 (for safe primes)
        mpz_tdiv_q_ui(q, p_minus_1, 2);
        mpz_powm(test, key->g, q, key->p);
        if (mpz_cmp_ui(test, 1) != 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        // Fallback: just use 2
        mpz_set_ui(key->g, 2);
    }

    // 3. Private key x: random in [1, p-2]
    mpz_urandomm(key->x, state, p_minus_1);
    if (mpz_cmp_ui(key->x, 0) == 0) mpz_set_ui(key->x, 1);

    // 4. Public key y = g^x mod p
    mpz_powm(key->y, key->g, key->x, key->p);

    key->bits = bits;

    mpz_clears(p_minus_1, q, test, NULL);
    gmp_randclear(state);
}

void elgamal_encrypt(ElGamalCiphertext* ct, const mpz_t message, const ElGamalKey* key)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    
    // Use time + a static counter to guarantee different seeds
    static unsigned long counter = 0;
    unsigned long seed = time(NULL) ^ (counter++ << 16);
    gmp_randseed_ui(state, seed);

    mpz_inits(ct->c1, ct->c2, NULL);

    mpz_t k, p_minus_1;
    mpz_inits(k, p_minus_1, NULL);
    mpz_sub_ui(p_minus_1, key->p, 1);
    mpz_urandomm(k, state, p_minus_1);
    if (mpz_cmp_ui(k, 0) == 0) mpz_set_ui(k, 1);

    // c1 = g^k mod p
    mpz_powm(ct->c1, key->g, k, key->p);

    // c2 = M * y^k mod p
    mpz_t yk;
    mpz_init(yk);
    mpz_powm(yk, key->y, k, key->p);
    mpz_mul(ct->c2, message, yk);
    mpz_mod(ct->c2, ct->c2, key->p);

    mpz_clears(k, p_minus_1, yk, NULL);
    gmp_randclear(state);
}




void elgamal_decrypt(mpz_t message, const ElGamalCiphertext* ct, const ElGamalKey* key)
{
    // M = c2 * (c1^x)^(-1) mod p
    mpz_t s, s_inv;
    mpz_inits(s, s_inv, NULL);

    // s = c1^x mod p
    mpz_powm(s, ct->c1, key->x, key->p);

    // s_inv = s^(-1) mod p
    mpz_invert(s_inv, s, key->p);

    // M = c2 * s_inv mod p
    mpz_mul(message, ct->c2, s_inv);
    mpz_mod(message, message, key->p);

    mpz_clears(s, s_inv, NULL);
}

void elgamal_free_key(ElGamalKey* key)
{
    mpz_clears(key->p, key->g, key->x, key->y, NULL);
}

void elgamal_free_ciphertext(ElGamalCiphertext* ct)
{
    mpz_clears(ct->c1, ct->c2, NULL);
}