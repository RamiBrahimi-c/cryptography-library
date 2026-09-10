#include <stdio.h>
#include <gmp.h>
#include "elgamal.h"



typedef struct {
    mpz_t p;      // large prime (> 512 bits)
    mpz_t g;      // generator
    mpz_t x;      // private key (random)
    mpz_t y;      // public key (y = g^x mod p)
    int bits;
} ElGamalKeyGmp;

typedef struct {
    mpz_t c1;     // g^k mod p
    mpz_t c2;     // M * y^k mod p
} ElGamalCiphertextGmp;

static void gmp_elgamal_generate_keypair(ElGamalKeyGmp* key, int bits);
static void gmp_elgamal_encrypt(ElGamalCiphertextGmp* ct, const mpz_t message, const ElGamalKeyGmp* key);
static void gmp_elgamal_decrypt(mpz_t message, const ElGamalCiphertextGmp* ct, const ElGamalKeyGmp* key);
static void gmp_elgamal_free_key(ElGamalKeyGmp* key);
static void gmp_elgamal_free_ciphertext(ElGamalCiphertextGmp* ct);




int main()
{
    printf("=== ElGamal Implementation ===\n\n");

    // 1 & 2: Key generation, encrypt M=12345, decrypt, verify
    printf("1. Key generation (> 512 bits)...\n");

    printf("============== GMP ====================\n") ;
    ElGamalKeyGmp key;
    gmp_elgamal_generate_keypair(&key, 1024);  // 1024-bit p
    
    gmp_printf("   p = %Zd\n", key.p);
    gmp_printf("   g = %Zd\n", key.g);
    gmp_printf("   x = %Zd\n", key.x);
    gmp_printf("   y = %Zd\n", key.y);
    
    printf("=======================================\n") ;
    printf("============== BIGRA9m ====================\n") ;
    ElGamalKey key2;
    elgamal_generate_keypair(&key2, 1024);  // 1024-bit p
    
    gmp_printf("   p =  ");
    bigra9m_print( key2.p) ; 
    gmp_printf("   g =  ");
    bigra9m_print( key2.g) ; 
    gmp_printf("   x =  ");
    bigra9m_print( key2.x) ; 
    gmp_printf("   y =  ");
    bigra9m_print( key2.y) ; 
    printf("===========================================\n") ;


    mpz_t M, decrypted;
    mpz_inits(M, decrypted, NULL);
    mpz_set_ui(M, 12345);

    printf("\n2. Encrypt M = 12345...\n");
    ElGamalCiphertextGmp ct;
    gmp_elgamal_encrypt(&ct, M, &key);
    gmp_printf("   C1 = %Zd\n", ct.c1);
    gmp_printf("   C2 = %Zd\n", ct.c2);

    gmp_elgamal_decrypt(decrypted, &ct, &key);
    gmp_printf("   Decrypted = %Zd\n", decrypted);
    printf("   D(E(M)) == M? %s\n", mpz_cmp(decrypted, M) == 0 ? "YES ✓" : "NO ✗");
    
    
    BigRa9m M2, decrypted2;
    bigra9m_inits(&M2, &decrypted2, NULL);
    bigra9m_assign_uint64_t(&M2, 12345);

    printf("\n2. Encrypt M = 12345...\n");
    ElGamalCiphertext ct2;

    elgamal_encrypt(&ct2, &M2, &key2);
    printf("   C1 = ");
    bigra9m_print( ct2.c1) ; 
    printf("   C2 = ");
    bigra9m_print( ct2.c2) ; 

    elgamal_decrypt(&decrypted2, &ct2, &key2);
    printf("   Decrypted = ");
    bigra9m_print(decrypted2) ; 
    printf("   D(E(M)) == M? %s\n", bigra9m_isEqualNum(decrypted2, M2) ? "YES ✓" : "NO ✗");


    // 2 (cont): Show non-determinism — encrypt same M twice
    printf("\n   Non-determinism GMP: encrypt M twice...\n");
    ElGamalCiphertextGmp ct2_gmp;
    gmp_elgamal_encrypt(&ct2_gmp, M, &key);
    gmp_printf("   First  C2 = %Zd\n", ct.c2);
    gmp_printf("   Second C2 = %Zd\n", ct2_gmp.c2);
    printf("   Same ciphertext? %s\n",
           (mpz_cmp(ct.c1, ct2_gmp.c1) == 0 && mpz_cmp(ct.c2, ct2_gmp.c2) == 0) ? "YES (bug)" : "NO ✓ (different)");
    gmp_elgamal_free_ciphertext(&ct2_gmp);



    printf("\n   Non-determinism BIGRA9M : encrypt M twice...\n");
    ElGamalCiphertext ct2_bigra9m;
    elgamal_encrypt(&ct2_bigra9m, &M2, &key2);
    printf("   First  C2 = ");
    bigra9m_print( ct2.c2) ; 
    printf("   Second C2 = ");
    bigra9m_print( ct2_bigra9m.c2) ; 
    printf("   Same ciphertext? %s\n",
           (bigra9m_isEqualNum(ct2.c1, ct2_bigra9m.c1) == 0 && bigra9m_isEqualNum(ct2.c2, ct2_bigra9m.c2) ) ? "YES (bug)" : "NO ✓ (different)");
    elgamal_free_ciphertext(&ct2_bigra9m);





    _exit(0) ; 

/*

    // 3: Malléabilité — forge E(2M) from E(M)
    printf("\n3. Malléabilité: forge E(2M) from E(M)...\n");
    ElGamalCiphertextGmp forged;
    mpz_inits(forged.c1, forged.c2, NULL);
    
    // forged.c1 = ct.c1 (same)
    mpz_set(forged.c1, ct.c1);
    // forged.c2 = 2 * ct.c2 mod p
    mpz_mul_ui(forged.c2, ct.c2, 2);
    mpz_mod(forged.c2, forged.c2, key.p);

    mpz_t M2, forged_decrypted;
    mpz_inits(M2, forged_decrypted, NULL);
    mpz_mul_ui(M2, M, 2);
    mpz_mod(M2, M2, key.p);   // M2 = 2M mod p

    gmp_elgamal_decrypt(forged_decrypted, &forged, &key);
    gmp_printf("   Expected 2M mod p = %Zd\n", M2);
    gmp_printf("   Decrypted forged = %Zd\n", forged_decrypted);
    printf("   Forged E(2M) decrypts to 2M? %s\n",
           mpz_cmp(forged_decrypted, M2) == 0 ? "YES ✓" : "NO ✗");

    mpz_clears(M2, forged_decrypted, NULL);
    gmp_elgamal_free_ciphertext(&forged);

printf("\n   Size comparison (both 2048-bit):\n");
printf("   RSA-2048 ciphertext: 256 bytes\n");
printf("   ElGamal-2048 ciphertext: 512 bytes (C1 + C2 = 2 × 256)\n");
printf("   ElGamal ciphertext is exactly double RSA's size.\n");
printf("   This is the cost of semantic security: every encryption\n");
printf("   uses a fresh random value k, requiring both C1 and C2.\n");
 */    // Cleanup
    mpz_clears(M, decrypted, NULL);
    gmp_elgamal_free_ciphertext(&ct);
    gmp_elgamal_free_key(&key);

    printf("\n=== Done ===\n");
    return 0;
}












//******************************************************************************************* */
//******************************************************************************************* */
//******************************************************************************************* */
//******************************************************************************************* */
//******************************************************************************************* */



static void gmp_elgamal_generate_keypair(ElGamalKeyGmp* key, int bits)
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


static void gmp_elgamal_encrypt(ElGamalCiphertextGmp* ct, const mpz_t message, const ElGamalKeyGmp* key)
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





static void gmp_elgamal_decrypt(mpz_t message, const ElGamalCiphertextGmp* ct, const ElGamalKeyGmp* key)
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


static void gmp_elgamal_free_key(ElGamalKeyGmp* key)
{
    mpz_clears(key->p, key->g, key->x, key->y, NULL);
}

static void gmp_elgamal_free_ciphertext(ElGamalCiphertextGmp* ct)
{
    mpz_clears(ct->c1, ct->c2, NULL);
}