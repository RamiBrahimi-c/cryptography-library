#include <gmp.h>
#include <stdlib.h>
#include <time.h>


#include "../../../include/ciphers/asymmetric/rsa.h"
#include "../../../include/ciphers/symmetric/aes.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/constants.h"

void rsa_generate_keypair(RsaKey* key, int bits, unsigned long e_value)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    unsigned long seed = time(NULL);
    if (seed == 0) seed = 42;
    gmp_randseed_ui(state, seed);

    mpz_t p, q, n, phi, p1, q1, gcd;
    mpz_inits(p, q, n, phi, p1, q1, gcd, NULL);

    int prime_bits = bits / 2;

    // Generate p
    mpz_urandomb(p, state, prime_bits);
    mpz_setbit(p, prime_bits - 1);  // ensure exactly prime_bits
    mpz_nextprime(p, p);

    // Generate q (different from p)
    do {
        mpz_urandomb(q, state, prime_bits);
        mpz_setbit(q, prime_bits - 1);
        mpz_nextprime(q, q);
    } while (mpz_cmp(p, q) == 0);

    // n = p * q
    mpz_mul(key->n, p, q);

    // phi = (p-1)*(q-1)
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // e
    mpz_set_ui(key->e, e_value);
    mpz_gcd(gcd, key->e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        mpz_set_ui(key->e, 65537);
    }

    // d = e⁻¹ mod phi
    mpz_invert(key->d, key->e, phi);

    key->bits = bits;
    key->is_private = 1;

    mpz_clears(p, q, n, phi, p1, q1, gcd, NULL);
    gmp_randclear(state);
}


void rsa_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const RsaKey* rsa = (const RsaKey*)key;
    size_t out_bytes = (rsa->bits + 7) / 8;
    memset(output, 0, out_bytes);

    mpz_t m, c;
    mpz_inits(m, c, NULL);
    mpz_import(m, length, 1, 1, 0, 0, input);
    mpz_powm(c, m, rsa->e, rsa->n);

    // Export to temporary buffer, then right-align into output
    size_t count = 0;
    unsigned char temp[1024];
    mpz_export(temp, &count, 1, 1, 0, 0, c);

    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }

    mpz_clears(m, c, NULL);
}

void rsa_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const RsaKey* rsa = (const RsaKey*)key;
    if (!rsa || !rsa->is_private || length <= 0) return;

    size_t out_bytes = (rsa->bits + 7) / 8;

    mpz_t c, m;
    mpz_inits(c, m, NULL);
    mpz_import(c, length, 1, 1, 0, 0, input);
    mpz_powm(m, c, rsa->d, rsa->n);

    // Export to temp, then copy right‑aligned into output buffer
    unsigned char temp[1024];   // up to 2048 bits = 256 bytes, safe
    size_t count = 0;
    mpz_export(temp, &count, 1, 1, 0, 0, m);

    // Zero the whole output buffer
    memset(output, 0, out_bytes);

    // Right‑align the actual bytes
    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        // Should not happen, but just copy the last out_bytes bytes
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }

    mpz_clears(c, m, NULL);
}



void rsa_set_key(void* key_struct, const char* key_str)
{
    RsaKey* rsa = (RsaKey*)key_struct;
    // parse "n_hex:e_hex:d_hex:bits"
    char* copy = strdup(key_str);
    char* tok_n = strtok(copy, ":");
    char* tok_e = strtok(NULL, ":");
    char* tok_d = strtok(NULL, ":");
    char* tok_bits = strtok(NULL, ":");
    if (tok_n && tok_e && tok_d && tok_bits) {
        mpz_set_str(rsa->n, tok_n, 16);
        mpz_set_str(rsa->e, tok_e, 16);
        mpz_set_str(rsa->d, tok_d, 16);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 1;
        printf("wth1\n") ; 
    } else if (tok_n && tok_e && tok_bits) {
        mpz_set_str(rsa->n, tok_n, 16);
        mpz_set_str(rsa->e, tok_e, 16);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 0;
        printf("wth2\n") ; 
    } else {
        printf("wth\n") ; 
    }
    free(copy);
}



void rsa_free_key(void* key_struct)
{
    RsaKey* rsa = (RsaKey*)key_struct;
    mpz_clears(rsa->n, rsa->e, rsa->d, NULL);
}



void rsa_export_key(const RsaKey* key) {
    printf("Modulus n (hex): ");
    mpz_out_str(stdout, 16, key->n);
    printf("\nPublic exponent e (hex): ");
    mpz_out_str(stdout, 16, key->e);
    if (key->is_private) {
        printf("\nPrivate exponent d (hex): ");
        mpz_out_str(stdout, 16, key->d);
    }
    printf("\nBits: %d\n", key->bits);
}

void test_rsa_32bytes(int bits) {
    printf("testing with %d bits\n", bits);
    fflush(stdout);

    RsaKey key;
    mpz_inits(key.n, key.e, key.d, NULL);

    rsa_generate_keypair(&key, bits, 65537);
    printf("generated key pairs successfully for %d bits\n", bits);
    fflush(stdout);

    uchar_t plain[32];
    for (int i = 0; i < 32; i++) plain[i] = (uchar_t)(i + 0x41);

    size_t out_bytes = (bits + 7) / 8;
    uchar_t* cipher = malloc(out_bytes);
    uchar_t* decrypted = malloc(out_bytes);

    rsa_encrypt(plain, cipher, 32, &key);
    rsa_decrypt(cipher, decrypted, out_bytes, &key);

    uchar_t recovered[32];
    memcpy(recovered, decrypted + (out_bytes - 32), 32);

    printf("RSA-%d match: %s\n", bits,
           memcmp(plain, recovered, 32) == 0 ? "YES" : "NO");
    fflush(stdout);

    free(cipher);
    free(decrypted);
    rsa_free_key(&key);
}
void sayhi() {
    printf("hi fuck this all\n") ; 
}

// test_rsa_32bytes(512), test_rsa_32bytes(1024), test_rsa_32bytes(2048)


void hybrid_test(void) {
    printf("hi") ;
    RsaKey rsa;
    mpz_inits(rsa.n, rsa.e, rsa.d, NULL);
    rsa_generate_keypair(&rsa, 2048, 65537);
    
    // 1. Random AES‑256 key (32 bytes)
    uchar_t aes_key[32];
    for (int i = 0; i < 32; i++) aes_key[i] = rand() & 0xFF;   // weak randomness
    
    // 2. Encrypt AES key with RSA
    size_t rsa_out = (rsa.bits + 7) / 8;
    uchar_t encrypted_key[rsa_out];
    clock_t t1 = clock();
    printf("hi\n") ;
    rsa_encrypt(aes_key,  encrypted_key , 32, &rsa);
    printf("hi!!!!!!!!!!!\n") ;
    clock_t t2 = clock();
    double rsa_time = (double)(t2 - t1) / CLOCKS_PER_SEC;

    // 3. Read 1 MB file (or create a dummy 1 MB buffer)
    size_t file_size = 1024 * 1024;
    uchar_t* file_data = malloc(file_size);
    memset(file_data, 'A', file_size);   // dummy content

    // 4. Encrypt with AES 
    AesKey aes_ctx;   // assuming AES struct
    aes_set_key(&aes_ctx, (const uchar_t*) aes_key , 32);

    size_t padded_size = file_size + (16 - (file_size % 16));   // PKCS#7 padding
    uchar_t* aes_cipher = malloc(padded_size);
    clock_t t3 = clock();
    aes_encrypt(file_data, aes_cipher, file_size, &aes_ctx);   // this will write padded_size bytes
    clock_t t4 = clock();
    double aes_time = (double)(t4 - t3) / CLOCKS_PER_SEC;

    printf("RSA key encryption time: %f s\n", rsa_time);
    printf("AES file encryption time: %f s\n", aes_time);

    free(file_data);
    free(aes_cipher);
    rsa_free_key(&rsa);
}