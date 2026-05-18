#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stddef.h>

#define MD5_DIGEST_LENGTH    16
#define SHA256_DIGEST_LENGTH 32
#define SHA512_DIGEST_LENGTH 64

void md5_hash(const uint8_t* data, size_t len, uint8_t digest[16]);
void sha256_hash(const uint8_t* data, size_t len, uint8_t digest[32]);
void sha512_hash(const uint8_t* data, size_t len, uint8_t digest[64]);

// Helper: compute hash of a file
void md5_file(const char* filename, uint8_t digest[16]);
void sha256_file(const char* filename, uint8_t digest[32]);

// Avalanche test: flip one bit in input, compare digests
double avalanche_test(const uint8_t* input, size_t len,
                      void (*hash_fn)(const uint8_t*, size_t, uint8_t*),
                      int digest_len);

// Benchmark: MB/s
double bench_hash(void (*hash_fn)(const uint8_t*, size_t, uint8_t*),
                  size_t total_bytes, int digest_len);

#endif