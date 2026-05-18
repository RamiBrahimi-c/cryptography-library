/* #include <stdio.h>
#include <gmp.h>
#include "elgamal.h"

int main()
{
    printf("=== ElGamal Implementation ===\n\n");

    // 1 & 2: Key generation, encrypt M=12345, decrypt, verify
    printf("1. Key generation (> 512 bits)...\n");
    ElGamalKey key;
    elgamal_generate_keypair(&key, 1024);  // 1024-bit p

    gmp_printf("   p = %Zd\n", key.p);
    gmp_printf("   g = %Zd\n", key.g);
    gmp_printf("   x = %Zd\n", key.x);
    gmp_printf("   y = %Zd\n", key.y);

    mpz_t M, decrypted;
    mpz_inits(M, decrypted, NULL);
    mpz_set_ui(M, 12345);

    printf("\n2. Encrypt M = 12345...\n");
    ElGamalCiphertext ct;
    elgamal_encrypt(&ct, M, &key);
    gmp_printf("   C1 = %Zd\n", ct.c1);
    gmp_printf("   C2 = %Zd\n", ct.c2);

    elgamal_decrypt(decrypted, &ct, &key);
    gmp_printf("   Decrypted = %Zd\n", decrypted);
    printf("   D(E(M)) == M? %s\n", mpz_cmp(decrypted, M) == 0 ? "YES ✓" : "NO ✗");

    // 2 (cont): Show non-determinism — encrypt same M twice
    printf("\n   Non-determinism: encrypt M twice...\n");
    ElGamalCiphertext ct2;
    elgamal_encrypt(&ct2, M, &key);
    gmp_printf("   First  C2 = %Zd\n", ct.c2);
    gmp_printf("   Second C2 = %Zd\n", ct2.c2);
    printf("   Same ciphertext? %s\n",
           (mpz_cmp(ct.c1, ct2.c1) == 0 && mpz_cmp(ct.c2, ct2.c2) == 0) ? "YES (bug)" : "NO ✓ (different)");
    elgamal_free_ciphertext(&ct2);

    // 3: Malléabilité — forge E(2M) from E(M)
    printf("\n3. Malléabilité: forge E(2M) from E(M)...\n");
    ElGamalCiphertext forged;
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

    elgamal_decrypt(forged_decrypted, &forged, &key);
    gmp_printf("   Expected 2M mod p = %Zd\n", M2);
    gmp_printf("   Decrypted forged = %Zd\n", forged_decrypted);
    printf("   Forged E(2M) decrypts to 2M? %s\n",
           mpz_cmp(forged_decrypted, M2) == 0 ? "YES ✓" : "NO ✗");

    mpz_clears(M2, forged_decrypted, NULL);
    elgamal_free_ciphertext(&forged);

printf("\n   Size comparison (both 2048-bit):\n");
printf("   RSA-2048 ciphertext: 256 bytes\n");
printf("   ElGamal-2048 ciphertext: 512 bytes (C1 + C2 = 2 × 256)\n");
printf("   ElGamal ciphertext is exactly double RSA's size.\n");
printf("   This is the cost of semantic security: every encryption\n");
printf("   uses a fresh random value k, requiring both C1 and C2.\n");
    // Cleanup
    mpz_clears(M, decrypted, NULL);
    elgamal_free_ciphertext(&ct);
    elgamal_free_key(&key);

    printf("\n=== Done ===\n");
    return 0;
} */