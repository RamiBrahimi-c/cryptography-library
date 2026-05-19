/* #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
// #include "hash.h"

#include "../include/ciphers/hashing/hash.h"
#include <time.h>


void print_digest(const char* label, const uint8_t* d, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++) printf("%02x", d[i]);
    printf(" (%d bits)\n", len * 8);
}

int main()
{
    // ============ EXERCISE 4.1 — MD5 ============
    printf("=== MD5 Tests ===\n\n");

    // Test 1: empty string
    uint8_t d[64];
    md5_hash((uint8_t*)"", 0, d);
    print_digest("MD5(\"\")", d, 16);

    // Test 2: 1 byte
    md5_hash((uint8_t*)"A", 1, d);
    print_digest("MD5(\"A\")", d, 16);

    // Test 3: 1 KB
    uint8_t* kb = malloc(1024);
    memset(kb, 'B', 1024);
    md5_hash(kb, 1024, d);
    print_digest("MD5(1KB)", d, 16);
    free(kb);

    // Test 4: 1 MB (just first and last 16 bytes to avoid huge output)
    printf("MD5(1MB): computing...\n");
    uint8_t* mb = malloc(1024 * 1024);
    memset(mb, 'C', 1024 * 1024);
    md5_hash(mb, 1024 * 1024, d);
    print_digest("MD5(1MB)", d, 16);
    free(mb);

    // Avalanche test on MD5
    uint8_t test_data[1024];
    memset(test_data, 0xAA, 1024);
    double av = avalanche_test(test_data, 1024, md5_hash, 16);
    printf("MD5 avalanche: %.1f%% bits changed\n\n", av);

    // ============ EXERCISE 4.2 — SHA-256 ============
    printf("=== SHA-256 Tests ===\n\n");

    sha256_hash((uint8_t*)"hello", 5, d);
    print_digest("SHA256(\"hello\")", d, 32);

    // 10 test vectors (abbreviated — just show a few)
    const char* vectors[] = {"", "a", "ab", "abc", "abcd", "hello", "world", "test", "12345", "abcdefgh"};
    for (int i = 0; i < 10; i++) {
        sha256_hash((uint8_t*)vectors[i], strlen(vectors[i]), d);
        printf("SHA256(\"%s\") = ", vectors[i]);
        for (int j = 0; j < 32; j++) printf("%02x", d[j]);
        printf("\n");
    }

    // Integrity check simulation
    printf("\nIntegrity check simulation:\n");
    // Create a dummy "downloaded" file
    FILE* f = fopen("/tmp/test_download.bin", "wb");
    for (int i = 0; i < 1000; i++) fputc(i % 256, f);
    fclose(f);

    uint8_t file_hash[32];
    sha256_file("/tmp/test_download.bin", file_hash);
    printf("Downloaded file SHA256: ");
    for (int i = 0; i < 32; i++) printf("%02x", file_hash[i]);
    printf("\n");

    // Simulate "official" hash (same)
    printf("Official hash:        ");
    for (int i = 0; i < 32; i++) printf("%02x", file_hash[i]);
    printf("\nStatus: OK ✓ (integrity verified)\n\n");

    // ============ EXERCISE 4.3 — Comparison ============
    printf("=== SHA-512 & Comparison ===\n\n");

    sha512_hash((uint8_t*)"hello", 5, d);
    print_digest("SHA512(\"hello\")", d, 64);

    // Compare all three on same message
    const char* msg = "The quick brown fox jumps over the lazy dog";
    printf("\nMessage: \"%s\"\n\n", msg);

    clock_t t0, t1;
    
    t0 = clock();
    md5_hash((uint8_t*)msg, strlen(msg), d);
    t1 = clock();
    print_digest("MD5   ", d, 16);
    printf("  Time: %.6f ms\n", (t1 - t0) * 1000.0 / CLOCKS_PER_SEC);

    t0 = clock();
    sha256_hash((uint8_t*)msg, strlen(msg), d);
    t1 = clock();
    print_digest("SHA256", d, 32);
    printf("  Time: %.6f ms\n", (t1 - t0) * 1000.0 / CLOCKS_PER_SEC);

    t0 = clock();
    sha512_hash((uint8_t*)msg, strlen(msg), d);
    t1 = clock();
    print_digest("SHA512", d, 64);
    printf("  Time: %.6f ms\n", (t1 - t0) * 1000.0 / CLOCKS_PER_SEC);

    // Avalanche for all three
    av = avalanche_test((uint8_t*)msg, strlen(msg), md5_hash, 16);
    printf("\nMD5    avalanche: %.1f%%\n", av);
    av = avalanche_test((uint8_t*)msg, strlen(msg), sha256_hash, 32);
    printf("SHA256 avalanche: %.1f%%\n", av);
    av = avalanche_test((uint8_t*)msg, strlen(msg), sha512_hash, 64);
    printf("SHA512 avalanche: %.1f%%\n", av);

    // Benchmark on 100 MB
    printf("\nBenchmark 100 MB:\n");
    double mbps;
    mbps = bench_hash(md5_hash, 100 * 1024 * 1024, 16);
    printf("MD5:    %.2f MB/s\n", mbps);
    mbps = bench_hash(sha256_hash, 100 * 1024 * 1024, 32);
    printf("SHA256: %.2f MB/s\n", mbps);
    mbps = bench_hash(sha512_hash, 100 * 1024 * 1024, 64);
    printf("SHA512: %.2f MB/s\n", mbps);

    // ============ HMAC + HKDF Known-Answer-Tests ============
    int kat_failures = 0;

    printf("\n=== HMAC-SHA256 KATs (RFC 4231) ===\n\n");

    // RFC 4231 Test Case 1
    {
        uint8_t key[20]; memset(key, 0x0b, 20);
        const uint8_t* data = (const uint8_t*)"Hi There";
        uint8_t mac[32];
        const uint8_t expected[32] = {
            0xb0,0x34,0x4c,0x61,0xd8,0xdb,0x38,0x53,0x5c,0xa8,0xaf,0xce,0xaf,0x0b,0xf1,0x2b,
            0x88,0x1d,0xc2,0x00,0xc9,0x83,0x3d,0xa7,0x26,0xe9,0x37,0x6c,0x2e,0x32,0xcf,0xf7
        };
        hmac_sha256(key, 20, data, 8, mac);
        int ok = ct_memcmp(mac, expected, 32) == 0;
        print_digest("Test 1 HMAC", mac, 32);
        printf("  Test 1: %s\n", ok ? "PASS" : "FAIL");
        if (!ok) kat_failures++;
    }

    // RFC 4231 Test Case 2 — short key
    {
        const uint8_t* key = (const uint8_t*)"Jefe";
        const uint8_t* data = (const uint8_t*)"what do ya want for nothing?";
        uint8_t mac[32];
        const uint8_t expected[32] = {
            0x5b,0xdc,0xc1,0x46,0xbf,0x60,0x75,0x4e,0x6a,0x04,0x24,0x26,0x08,0x95,0x75,0xc7,
            0x5a,0x00,0x3f,0x08,0x9d,0x27,0x39,0x83,0x9d,0xec,0x58,0xb9,0x64,0xec,0x38,0x43
        };
        hmac_sha256(key, 4, data, 28, mac);
        int ok = ct_memcmp(mac, expected, 32) == 0;
        printf("  Test 2: %s\n", ok ? "PASS" : "FAIL");
        if (!ok) kat_failures++;
    }

    // RFC 4231 Test Case 6 — key larger than block size (131 bytes)
    {
        uint8_t key[131]; memset(key, 0xaa, 131);
        const uint8_t* data = (const uint8_t*)
            "Test Using Larger Than Block-Size Key - Hash Key First";
        uint8_t mac[32];
        const uint8_t expected[32] = {
            0x60,0xe4,0x31,0x59,0x1e,0xe0,0xb6,0x7f,0x0d,0x8a,0x26,0xaa,0xcb,0xf5,0xb7,0x7f,
            0x8e,0x0b,0xc6,0x21,0x37,0x28,0xc5,0x14,0x05,0x46,0x04,0x0f,0x0e,0xe3,0x7f,0x54
        };
        hmac_sha256(key, 131, data, 54, mac);
        int ok = ct_memcmp(mac, expected, 32) == 0;
        printf("  Test 6 (oversized key): %s\n", ok ? "PASS" : "FAIL");
        if (!ok) kat_failures++;
    }

    printf("\n=== HMAC-SHA512 KATs (RFC 4231) ===\n\n");

    // RFC 4231 Test Case 1 for SHA-512
    {
        uint8_t key[20]; memset(key, 0x0b, 20);
        const uint8_t* data = (const uint8_t*)"Hi There";
        uint8_t mac[64];
        const uint8_t expected[64] = {
            0x87,0xaa,0x7c,0xde,0xa5,0xef,0x61,0x9d,0x4f,0xf0,0xb4,0x24,0x1a,0x1d,0x6c,0xb0,
            0x23,0x79,0xf4,0xe2,0xce,0x4e,0xc2,0x78,0x7a,0xd0,0xb3,0x05,0x45,0xe1,0x7c,0xde,
            0xda,0xa8,0x33,0xb7,0xd6,0xb8,0xa7,0x02,0x03,0x8b,0x27,0x4e,0xae,0xa3,0xf4,0xe4,
            0xbe,0x9d,0x91,0x4e,0xeb,0x61,0xf1,0x70,0x2e,0x69,0x6c,0x20,0x3a,0x12,0x68,0x54
        };
        hmac_sha512(key, 20, data, 8, mac);
        int ok = ct_memcmp(mac, expected, 64) == 0;
        printf("  Test 1: %s\n", ok ? "PASS" : "FAIL");
        if (!ok) kat_failures++;
    }

    printf("\n=== HKDF-SHA256 KATs (RFC 5869) ===\n\n");

    // RFC 5869 Test Case 1 (basic)
    {
        uint8_t ikm[22]; memset(ikm, 0x0b, 22);
        const uint8_t salt[13] = {
            0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c
        };
        const uint8_t info[10] = {
            0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9
        };
        const uint8_t expected_prk[32] = {
            0x07,0x77,0x09,0x36,0x2c,0x2e,0x32,0xdf,0x0d,0xdc,0x3f,0x0d,0xc4,0x7b,0xba,0x63,
            0x90,0xb6,0xc7,0x3b,0xb5,0x0f,0x9c,0x31,0x22,0xec,0x84,0x4a,0xd7,0xc2,0xb3,0xe5
        };
        const uint8_t expected_okm[42] = {
            0x3c,0xb2,0x5f,0x25,0xfa,0xac,0xd5,0x7a,0x90,0x43,0x4f,0x64,0xd0,0x36,0x2f,0x2a,
            0x2d,0x2d,0x0a,0x90,0xcf,0x1a,0x5a,0x4c,0x5d,0xb0,0x2d,0x56,0xec,0xc4,0xc5,0xbf,
            0x34,0x00,0x72,0x08,0xd5,0xb8,0x87,0x18,0x58,0x65
        };

        uint8_t prk[32], okm[42];
        hkdf_extract_sha256(salt, sizeof salt, ikm, sizeof ikm, prk);
        int prk_ok = ct_memcmp(prk, expected_prk, 32) == 0;
        printf("  Test 1 Extract (PRK): %s\n", prk_ok ? "PASS" : "FAIL");
        if (!prk_ok) kat_failures++;

        int rc = hkdf_expand_sha256(prk, info, sizeof info, okm, sizeof okm);
        int okm_ok = rc == 0 && ct_memcmp(okm, expected_okm, 42) == 0;
        printf("  Test 1 Expand (OKM): %s\n", okm_ok ? "PASS" : "FAIL");
        if (!okm_ok) kat_failures++;

        // Combined API should match
        uint8_t okm2[42];
        rc = hkdf_sha256(salt, sizeof salt, ikm, sizeof ikm,
                        info, sizeof info, okm2, sizeof okm2);
        int combined_ok = rc == 0 && ct_memcmp(okm2, expected_okm, 42) == 0;
        printf("  Test 1 Combined:     %s\n", combined_ok ? "PASS" : "FAIL");
        if (!combined_ok) kat_failures++;
    }

    // RFC 5869 Test Case 3 — empty salt and info, L=42
    {
        uint8_t ikm[22]; memset(ikm, 0x0b, 22);
        const uint8_t expected_prk[32] = {
            0x19,0xef,0x24,0xa3,0x2c,0x71,0x7b,0x16,0x7f,0x33,0xa9,0x1d,0x6f,0x64,0x8b,0xdf,
            0x96,0x59,0x67,0x76,0xaf,0xdb,0x63,0x77,0xac,0x43,0x4c,0x1c,0x29,0x3c,0xcb,0x04
        };
        const uint8_t expected_okm[42] = {
            0x8d,0xa4,0xe7,0x75,0xa5,0x63,0xc1,0x8f,0x71,0x5f,0x80,0x2a,0x06,0x3c,0x5a,0x31,
            0xb8,0xa1,0x1f,0x5c,0x5e,0xe1,0x87,0x9e,0xc3,0x45,0x4e,0x5f,0x3c,0x73,0x8d,0x2d,
            0x9d,0x20,0x13,0x95,0xfa,0xa4,0xb6,0x1a,0x96,0xc8
        };

        uint8_t prk[32], okm[42];
        hkdf_extract_sha256(NULL, 0, ikm, sizeof ikm, prk);
        int prk_ok = ct_memcmp(prk, expected_prk, 32) == 0;
        printf("  Test 3 Extract (empty salt): %s\n", prk_ok ? "PASS" : "FAIL");
        if (!prk_ok) kat_failures++;

        int rc = hkdf_expand_sha256(prk, NULL, 0, okm, sizeof okm);
        int okm_ok = rc == 0 && ct_memcmp(okm, expected_okm, 42) == 0;
        printf("  Test 3 Expand  (empty info): %s\n", okm_ok ? "PASS" : "FAIL");
        if (!okm_ok) kat_failures++;
    }

    // Length boundary: L > 255*HashLen must fail.
    {
        uint8_t prk[32] = {0};
        uint8_t big[255 * 32 + 1];
        int rc = hkdf_expand_sha256(prk, NULL, 0, big, sizeof big);
        printf("  Length-limit rejection: %s\n", rc == -1 ? "PASS" : "FAIL");
        if (rc != -1) kat_failures++;
    }

    // ct_memcmp sanity: equal -> 0, single-bit diff -> nonzero.
    {
        uint8_t a[32], b[32];
        memset(a, 0x42, 32);
        memcpy(b, a, 32);
        int eq = ct_memcmp(a, b, 32);
        b[17] ^= 0x01;
        int ne = ct_memcmp(a, b, 32);
        int ok = (eq == 0) && (ne != 0);
        printf("  ct_memcmp sanity:    %s\n", ok ? "PASS" : "FAIL");
        if (!ok) kat_failures++;
    }

    printf("\n=== KAT summary: %s (%d failure%s) ===\n",
           kat_failures == 0 ? "ALL PASS" : "FAILURES",
           kat_failures, kat_failures == 1 ? "" : "s");

    printf("\n=== Done ===\n");
    return kat_failures == 0 ? 0 : 1;
}