#include "hash.h"

#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// Count differing bits between two digests
static int count_diff_bits(const uint8_t* d1, const uint8_t* d2, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++) {
        uint8_t diff = d1[i] ^ d2[i];
        // Count set bits
        for (int b = 0; b < 8; b++)
            if (diff & (1 << b)) count++;
    }
    return count;
}

double avalanche_test(const uint8_t* input, size_t len,
                      void (*hash_fn)(const uint8_t*, size_t, uint8_t*),
                      int digest_len)
{
    uint8_t* modified = malloc(len);
    memcpy(modified, input, len);

    uint8_t digest1[64], digest2[64];
    hash_fn(input, len, digest1);

    // Total bits across many runs
    int total_diff = 0;
    int runs = 100;

    for (int r = 0; r < runs; r++) {
        // Flip a random bit
        memcpy(modified, input, len);
        int byte_idx = rand() % len;
        int bit_idx = rand() % 8;
        modified[byte_idx] ^= (1 << bit_idx);

        hash_fn(modified, len, digest2);
        total_diff += count_diff_bits(digest1, digest2, digest_len);
    }

    free(modified);
    return (double)total_diff / (runs * digest_len * 8) * 100.0;
}

double bench_hash(void (*hash_fn)(const uint8_t*, size_t, uint8_t*),
                  size_t total_bytes, int digest_len)
{
    uint8_t* data = malloc(total_bytes);
    memset(data, 'A', total_bytes);

    uint8_t digest[64];
    clock_t start = clock();
    hash_fn(data, total_bytes, digest);
    clock_t end = clock();

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    double mb_per_sec = (total_bytes / (1024.0 * 1024.0)) / seconds;

    free(data);
    return mb_per_sec;
}


// HKDF-Extract (RFC 5869 §2.2): PRK = HMAC-Hash(salt, IKM).
// A NULL/empty salt is replaced by HashLen zero bytes per the spec.
void hkdf_extract_sha256(const uint8_t* salt, size_t salt_len,
                         const uint8_t* ikm, size_t ikm_len,
                         uint8_t prk[32])
{
    if (salt == NULL || salt_len == 0) {
        uint8_t zero_salt[SHA256_DIGEST_LENGTH] = {0};
        hmac_sha256(zero_salt, SHA256_DIGEST_LENGTH, ikm, ikm_len, prk);
    } else {
        hmac_sha256(salt, salt_len, ikm, ikm_len, prk);
    }
}

// HKDF-Expand (RFC 5869 §2.3).
// T(0) = empty; T(i) = HMAC(PRK, T(i-1) || info || i); OKM = T(1) || T(2) || ... truncated.
int hkdf_expand_sha256(const uint8_t prk[32],
                       const uint8_t* info, size_t info_len,
                       uint8_t* okm, size_t okm_len)
{
    const size_t hash_len = SHA256_DIGEST_LENGTH;
    if (okm_len > 255 * hash_len) return -1;

    size_t n = (okm_len + hash_len - 1) / hash_len;
    uint8_t t[SHA256_DIGEST_LENGTH];
    size_t t_len = 0;
    size_t written = 0;

    for (size_t i = 1; i <= n; i++) {
        // Build HMAC input incrementally to avoid a per-iteration malloc.
        uint8_t k_block[SHA256_BLOCK_SIZE];
        uint8_t k_ipad[SHA256_BLOCK_SIZE];
        uint8_t k_opad[SHA256_BLOCK_SIZE];
        uint8_t inner[SHA256_DIGEST_LENGTH];

        // PRK is exactly 32 bytes — always fits in the block, no rehash needed.
        memset(k_block, 0, SHA256_BLOCK_SIZE);
        memcpy(k_block, prk, SHA256_DIGEST_LENGTH);

        for (size_t b = 0; b < SHA256_BLOCK_SIZE; b++) {
            k_ipad[b] = k_block[b] ^ 0x36;
            k_opad[b] = k_block[b] ^ 0x5c;
        }

        uint8_t counter = (uint8_t)i;
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, k_ipad, SHA256_BLOCK_SIZE);
        if (t_len > 0) SHA256_Update(&ctx, t, t_len);
        if (info_len > 0) SHA256_Update(&ctx, info, info_len);
        SHA256_Update(&ctx, &counter, 1);
        SHA256_Final(inner, &ctx);

        SHA256_Init(&ctx);
        SHA256_Update(&ctx, k_opad, SHA256_BLOCK_SIZE);
        SHA256_Update(&ctx, inner, SHA256_DIGEST_LENGTH);
        SHA256_Final(t, &ctx);
        t_len = SHA256_DIGEST_LENGTH;

        size_t remaining = okm_len - written;
        size_t to_copy = remaining < hash_len ? remaining : hash_len;
        memcpy(okm + written, t, to_copy);
        written += to_copy;
    }
    return 0;
}

int hkdf_sha256(const uint8_t* salt, size_t salt_len,
                const uint8_t* ikm, size_t ikm_len,
                const uint8_t* info, size_t info_len,
                uint8_t* okm, size_t okm_len)
{
    uint8_t prk[SHA256_DIGEST_LENGTH];
    hkdf_extract_sha256(salt, salt_len, ikm, ikm_len, prk);
    return hkdf_expand_sha256(prk, info, info_len, okm, okm_len);
}

// Constant-time compare: OR all byte differences, never short-circuit.
// Returns 0 iff equal — preserves memcmp semantics for the equality case.
int ct_memcmp(const void* a, const void* b, size_t len)
{
    const uint8_t* pa = (const uint8_t*)a;
    const uint8_t* pb = (const uint8_t*)b;
    uint8_t diff = 0;
    for (size_t i = 0; i < len; i++) diff |= (uint8_t)(pa[i] ^ pb[i]);
    return diff;
}