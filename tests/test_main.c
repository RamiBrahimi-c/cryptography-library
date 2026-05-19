#include "affine.h"
#include "hill.h"
#include "substitution.h"
#include "rc4.h"
#include "des.h"
#include "aes.h"
#include "aes_lib.h"
#include "rsa.h"
#include "elgamal.h"
#include "dh.h"

#include "test_utils.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.c"

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"



void strip_extension(char *filename) {
    // Find the last dot in the filename
    char *dot = strrchr(filename, '.');
    
    // If a dot exists and it's not the first character (e.g., ".gitignore")
    if (dot != NULL && dot != filename) {
        *dot = '\0'; // Replace dot with null terminator
    }
}


void setupFullFilePath(char *directory_name , char *filename , char *full_path , int total_size  ) {
    snprintf(full_path ,total_size, "./%s/%s" , directory_name , filename )  ; 
}

void setupFullResultFilePath(char *directory_name , char *type_algo  , char *filename , char *algo_name , char *full_path ,  int total_size , char *extension ) {
    
    // NOTE: just to force the output to be .png
    // NOTE: maybe add full date and time
    char filename_copy[64] ; 
    strcpy(filename_copy , filename) ; 

    strip_extension(filename_copy) ;     
    strcat(filename_copy ,  extension) ; 
    
    snprintf(full_path , total_size , "./%s/%s_%s_%s" , directory_name , type_algo   , algo_name, filename_copy )  ; 

}

int main() {
    char *_filename = "coast.jpg";
    
    // --- Symmetric ---
    uchar_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    TEST_ON_IMAGE_ENCRYPTION(aes, _filename, key, AesKey);
    TEST_ON_IMAGE_ENCRYPTION(des, _filename, key, DesKey);
    TEST_ON_IMAGE_ENCRYPTION(affine, _filename, "19", AffineKey);
    TEST_ON_IMAGE_ENCRYPTION(hill, _filename, "2", HillKey);
    TEST_ON_IMAGE_ENCRYPTION(rc4, _filename, "keykey", Rc4Key);

    // --- Asymmetric ---
    TEST_RSA_ENCRYPT_IMAGE(rsa, _filename, 512);
    TEST_RSA_ENCRYPT_IMAGE(rsa, _filename, 1024);
    TEST_RSA_ENCRYPT_IMAGE(rsa, _filename, 2048);

    // --- ElGamal non-determinism ---
    TEST_ELGAMAL_NONDETERMINISM();

    // --- DH + MITM ---
    TEST_DH_MITM();

    return 0;
}


// TEST(affine_encrypt_basic) {
//     char output[100];
//     AffineKey key = {5, 8};  // a=5, b=8
    
//     affine_encrypt("HELLO", output , 5, &key);
//     ASSERT_STR_EQ(output, "RCLLA");
//     return 0;
// }

// TEST(affine_invalid_key) {
//     AffineKey key = {2, 3};  // a=2 (invalid - not coprime to 26)
//     // Should handle error case
//     ASSERT_TRUE(is_valid_affine_key(&key) == 0);
//     return 0;
// }

// int main() {
//     RUN_TEST(affine_encrypt);
//     // RUN_TEST(affine_invalid_key);
//     return 0;
// }
