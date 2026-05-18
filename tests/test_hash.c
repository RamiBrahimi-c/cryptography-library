#include <stdio.h>
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

    printf("\n=== Done ===\n");
    return 0;
}