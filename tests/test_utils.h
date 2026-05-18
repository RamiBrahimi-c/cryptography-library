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

#define PRINT_ARRAY(array , length) \
    do { \
        printf("Array (%d):\n", length); \
        for (int _i = 0; _i < (length); _i++) { \
                printf("%2x ", (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)


#define TEST_ON_TEXT_ENCRYPTION(name , _original_text  , _length, _key , _key_type ) do { \
    printf( "INFO :%sTesting %s... %s\n" , COLOR_GREEN , #name , COLOR_RESET); \
    int length = _length;\
    uchar_t encrypted_text[MAX_LENGTH_TEXT_TEST];\
    void *key_##name = calloc(1 , sizeof(_key_type)) ;\
    ASSERT_NOT_NULL(key_##name);\
    printf("INFO: setting key... \n");\
    name##_set_key(key_##name , _key);\
    printf("INFO: key set with success\n");\
    name##_encrypt(_original_text ,encrypted_text  ,length , key_##name );\
    \
    printf("INFO: encrypted with success\n");\
    printf("INFO: encrypted text : %s \n" ,encrypted_text );\
    PRINT_ARRAY(encrypted_text , length);\
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






#endif