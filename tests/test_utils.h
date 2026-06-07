#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FULL_PATH_LENGTH 128



void strip_extension(char *filename) ;

void setupFullFilePath(char *directory_name , char *filename , char *full_path , int total_size  ) ;

void setupFullResultFilePath(char *directory_name , char *type_algo  , char *filename , char *algo_name , char *full_path ,  int total_size  , char *extension) ;



// Test runner macros
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    printf("Running %s... ", #name); \
    if (test_##name() == 0) \
    printf("PASSED\n"); \
    else \
    printf("FAILED\n"); \
} while(0)

#define MAX_LENGTH_TEXT_TEST 2048

// Assertions
#define ASSERT_EQ(actual, expected) \
    if ((actual) != (expected)) { \
        printf("\n  ASSERT_EQ failed: %d != %d (line %d)\n", \
               (actual), (expected), __LINE__); \
        return 1; \
    }

#define ASSERT_STR_EQ(actual, expected) \
    if (strcmp(actual, expected) != 0) { \
        printf("\n  ASSERT_STR_EQ failed: \"%s\" != \"%s\" (line %d)\n", \
               (actual), (expected), __LINE__); \
        return 1; \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        printf("\n  ASSERT_TRUE failed (line %d)\n", __LINE__); \
        return 1; \
    }

#define ASSERT_NOT_NULL(ptr) \
    if ((ptr) == NULL) { \
        printf("\n  ASSERT_NOT_NULL failed (line %d)\n", __LINE__); \
        return 1; \
    }

// Test colors (optional)
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"

// Test summary tracker
typedef struct {
    int passed;
    int failed;
} TestResult;

#define TEST_START() TestResult result = {0, 0}
#define TEST_PASS() result.passed++
#define TEST_FAIL() result.failed++
#define TEST_SUMMARY() \
    printf("\nSummary: %d passed, %d failed\n", result.passed, result.failed)



typedef unsigned char uchar_t ; 




#define TEST_ON_TEXT_ENCRYPTION(name , _original_text , _encrypted_text , _length, _key , _key_type ) do { \
    printf( "INFO :%sTesting %s... %s\n" , COLOR_GREEN , #name , COLOR_RESET); \
    int length = _length;\
    void *key_##name = calloc(1 , sizeof(_key_type)) ;\
    ASSERT_NOT_NULL(key_##name);\
    printf("INFO: setting key... \n");\
    name##_set_key(key_##name , _key);\
    printf("INFO: key set with success\n");\
    name##_encrypt(_original_text ,_encrypted_text  ,length , key_##name );\
    \
    printf("INFO: encrypted with success\n");\
    \
    \
} while(0)






#define TEST_ON_IMAGE_ENCRYPTION(name , _filename , _key , _key_type) do { \
    printf( "INFO :%sTesting %s... %s\n" , COLOR_GREEN , #name , COLOR_RESET); \
    printf( "INFO : Image \n"  ); \
    \
    int width, height, channels;\
    char *filename = _filename ; \
    char *directory_input_images = "tests/img" ; \
    char *directory_output_images = "tests/results_img" ;\ 
    char *algo_name = #name ; \
    char *enc_type_algo = "enc" ; \
    char *dec_type_algo = "dec" ; \
    char full_path_image_file[FULL_PATH_LENGTH]  ; \
    char full_path_result_image_file[FULL_PATH_LENGTH]  ;\
    setupFullFilePath(directory_input_images, filename ,full_path_image_file  , FULL_PATH_LENGTH ) ; \
    setupFullResultFilePath(directory_output_images , enc_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH , ".png") ;\
    uchar_t *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0);\
    ASSERT_NOT_NULL(original_text);\
    int length = width * height * channels ; \
    uchar_t *encrypted_text = malloc(sizeof(uchar_t) * length );\
    ASSERT_NOT_NULL(encrypted_text);\
    void *key_##name = calloc(1 , sizeof(_key_type)) ;\
    ASSERT_NOT_NULL(key_##name);\
    printf("INFO: setting key... \n");\
    name##_set_key(key_##name , _key);\
    printf("INFO: key set with success\n");\
    name##_encrypt(original_text ,encrypted_text  ,length , key_##name );\
    printf("INFO: encrypted with success\n");\
    \
    stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels);\
    printf("INFO: saved to %s \n" , full_path_result_image_file);\
    \
    free(key_##name);\
    free(encrypted_text);\
    \
    stbi_image_free(original_text);\
    \
} while(0)




#define TEST_ON_SOUND_ENCRYPTION(name , _filename , _key , _key_type) do { \
    printf( "INFO :%sTesting %s... %s\n" , COLOR_GREEN , #name , COLOR_RESET); \
    printf( "INFO : Sound \n" ); \
    int channels, sample_rate;\
    char *filename = _filename ; \
    char *directory_input_sounds = "tests/sound" ; \
    char *directory_output_sounds = "tests/results_sound" ;\ 
    char *algo_name = #name ; \
    char *enc_type_algo = "enc" ; \
    char *dec_type_algo = "dec" ; \
    char full_path_sound_file[FULL_PATH_LENGTH]  ; \
    char full_path_result_sound_file[FULL_PATH_LENGTH]  ;\
    setupFullFilePath(directory_input_sounds, filename ,full_path_sound_file  , FULL_PATH_LENGTH ) ; \
    setupFullResultFilePath(directory_output_sounds , enc_type_algo , filename , algo_name ,full_path_result_sound_file  , FULL_PATH_LENGTH , ".raw") ;\
    \
    short *audio_data;\
    int samples = stb_vorbis_decode_filename(\
        full_path_sound_file, \
        &channels, \
        &sample_rate,\
        &audio_data\
    );\
    if (samples < 0)\
    {\
        printf("%sERROR%s:FILE NOT FOUND\n" , COLOR_RED , COLOR_RESET);\
        break;\
    }\
    \
    printf("INFO: Audio: %d samples, %d channels, %d Hz\n", \
           samples, channels, sample_rate);\
    printf("INFO: Total shorts: %d\n", samples * channels);\
    \
    unsigned char *byte_data = (unsigned char *)audio_data;\
        \
    int total_bytes = samples * channels * sizeof(short);\
        \
    void *key_##name = malloc(sizeof(_key_type)  ) ;\
    printf("INFO: setting key... \n");\
    name##_set_key(key_##name , _key);\
    printf("INFO: key set with success\n");\
    \
    unsigned char *encrypted_text = malloc(sizeof(char) * total_bytes) ;\
    name##_encrypt(byte_data , encrypted_text , total_bytes , key_##name) ; \
    printf("INFO: encrypted with success\n");\
     \
    FILE *ptr = fopen( full_path_result_sound_file, "wb+") ; \
    ASSERT_NOT_NULL(ptr); \
    fwrite(encrypted_text , sizeof(char) , total_bytes , ptr) ; \
    printf("INFO: saved to %s \n" , full_path_result_sound_file);\
    free(audio_data);\
    \
    \
    free(key_##name);\
    free(encrypted_text);\
    \
    \
} while(0)



// ============ ASYMMETRIC TEST MACROS ============

// For RSA: key is generated, not passed as string
#define TEST_RSA_ENCRYPT_IMAGE(name, _filename, _bits) do { \
    printf("INFO: %sTesting %s-%d... %s\n", COLOR_GREEN, #name, _bits, COLOR_RESET); \
    printf("INFO: Image\n"); \
    \
    int width, height, channels; \
    char *filename = _filename; \
    char *directory_input_images = "tests/img"; \
    char *directory_output_images = "tests/results_img"; \
    char *algo_name = #name; \
    char *enc_type_algo = "enc"; \
    char full_path_image_file[FULL_PATH_LENGTH]; \
    char full_path_result_image_file[FULL_PATH_LENGTH]; \
    char algo_label[64]; \
    snprintf(algo_label, 64, "%s_%d", #name, _bits); \
    setupFullFilePath(directory_input_images, filename, full_path_image_file, FULL_PATH_LENGTH); \
    setupFullResultFilePath(directory_output_images, enc_type_algo, filename, algo_label, full_path_result_image_file, FULL_PATH_LENGTH, ".png"); \
    \
    uchar_t *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0); \
    ASSERT_NOT_NULL(original_text); \
    int length = width * height * channels; \
    \
    RsaKey key; \
    mpz_inits(key.n, key.e, key.d, NULL); \
    rsa_generate_keypair(&key, _bits, 65537); \
    printf("INFO: RSA-%d key generated\n", _bits); \
    \
    /* RSA encrypts in blocks of (bits/8 - 1) bytes max */ \
    size_t block_size = (_bits / 8) - 1; \
    size_t out_block = (_bits + 7) / 8; \
    size_t num_blocks = (length + block_size - 1) / block_size; \
    size_t total_out = num_blocks * out_block; \
    \
    uchar_t *encrypted_text = calloc(1, total_out); \
    ASSERT_NOT_NULL(encrypted_text); \
    \
    for (size_t i = 0; i < num_blocks; i++) { \
        size_t in_offset = i * block_size; \
        size_t this_block = (in_offset + block_size <= (size_t)length) ? block_size : (length - in_offset); \
        uchar_t block[256] = {0}; \
        memcpy(block, original_text + in_offset, this_block); \
        uchar_t out[512]; \
        rsa_encrypt(block, out, this_block, &key); \
        memcpy(encrypted_text + i * out_block, out, out_block); \
    } \
    printf("INFO: encrypted with success (%zu blocks)\n", num_blocks); \
    \
    /* Save as image (just the encrypted bytes reshaped) */ \
    stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels); \
    printf("INFO: saved to %s\n", full_path_result_image_file); \
    \
    rsa_free_key(&key); \
    free(encrypted_text); \
    stbi_image_free(original_text); \
} while(0)


// For ElGamal: encrypt 32-byte message, show non-determinism
#define TEST_ELGAMAL_NONDETERMINISM() do { \
    printf("INFO: %sTesting ElGamal non-determinism... %s\n", COLOR_GREEN, COLOR_RESET); \
    \
    ElGamalKey key; \
    elgamal_generate_keypair(&key, 1024); \
    \
    mpz_t M, dec1, dec2; \
    mpz_inits(M, dec1, dec2, NULL); \
    mpz_set_ui(M, 12345); \
    \
    ElGamalCiphertext ct1, ct2; \
    elgamal_encrypt(&ct1, M, &key); \
    elgamal_encrypt(&ct2, M, &key); \
    \
    elgamal_decrypt(dec1, &ct1, &key); \
    elgamal_decrypt(dec2, &ct2, &key); \
    \
    printf("  M = "); mpz_out_str(stdout, 10, M); printf("\n"); \
    printf("  Decrypt ct1 = "); mpz_out_str(stdout, 10, dec1); printf("\n"); \
    printf("  Decrypt ct2 = "); mpz_out_str(stdout, 10, dec2); printf("\n"); \
    printf("  Ciphertexts different? %s\n", \
           (mpz_cmp(ct1.c2, ct2.c2) != 0) ? "YES (non-deterministic)" : "NO (bug!)"); \
    \
    elgamal_free_ciphertext(&ct1); \
    elgamal_free_ciphertext(&ct2); \
    mpz_clears(M, dec1, dec2, NULL); \
    elgamal_free_key(&key); \
} while(0)


// For DH: show key exchange + MITM
#define TEST_DH_MITM() do { \
    printf("INFO: %sTesting DH + MITM... %s\n", COLOR_GREEN, COLOR_RESET); \
    \
    mpz_t p, g; \
    mpz_inits(p, g, NULL); \
    dh_generate_params(p, g, 512); \
    \
    DHParty alice, bob, mallory_a, mallory_b; \
    mpz_inits(alice.p, alice.private_key, alice.public_key, NULL); \
    mpz_inits(bob.p, bob.private_key, bob.public_key, NULL); \
    mpz_inits(mallory_a.p, mallory_a.private_key, mallory_a.public_key, NULL); \
    mpz_inits(mallory_b.p, mallory_b.private_key, mallory_b.public_key, NULL); \
    \
    dh_generate_keypair(&alice, p, g); \
    dh_generate_keypair(&bob, p, g); \
    dh_generate_keypair(&mallory_a, p, g); \
    dh_generate_keypair(&mallory_b, p, g); \
    \
    mpz_t K_alice, K_bob, K_mallory_a, K_mallory_b; \
    mpz_inits(K_alice, K_bob, K_mallory_a, K_mallory_b, NULL); \
    \
    /* Alice thinks she talks to Bob, actually talks to Mallory */ \
    dh_compute_shared(K_alice, &alice, mallory_b.public_key); \
    dh_compute_shared(K_bob, &bob, mallory_a.public_key); \
    dh_compute_shared(K_mallory_a, &mallory_a, alice.public_key); \
    dh_compute_shared(K_mallory_b, &mallory_b, bob.public_key); \
    \
    printf("  Alice key == Bob key? %s\n", mpz_cmp(K_alice, K_bob) == 0 ? "YES" : "NO (MITM active!)"); \
    printf("  Mallory has both keys? %s\n", \
           (mpz_cmp(K_mallory_a, K_alice) == 0 && mpz_cmp(K_mallory_b, K_bob) == 0) ? "YES" : "NO"); \
    \
    mpz_clears(p, g, K_alice, K_bob, K_mallory_a, K_mallory_b, NULL); \
    dh_clear_party(&alice); dh_clear_party(&bob); \
    dh_clear_party(&mallory_a); dh_clear_party(&mallory_b); \
} while(0)


#endif