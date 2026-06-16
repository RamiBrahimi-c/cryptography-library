#include "affine.h"
#include "hill.h"
#include "substitution.h"
#include "rc4.h"
#include "des.h"
#include "aes.h"
#include "rsa.h"
#include "elgamal.h"
#include "dh.h"
#include "hash.h"
#include "../include/common/utils.h"

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


#include "aes.h"




int main() {

    uchar_t original_cipher[64]  = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96
        ,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a
        ,0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c
        ,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51
        ,0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11
        ,0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef
        ,0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17
        ,0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10}; 
    
    
    uchar_t encrypted_cipher[64] ; 
    uchar_t decrypted_cipher[64] ; 

    uchar_t original_key[24] =  {
        0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };


        printf("hello there !!!!\n") ; 


    uchar_t input[] = "a" ;
    uint64_t length = strlen(input) ;

    printf("input : \n" );
    PRINT_ARRAY(input , length , "%c");
    PRINT_ARRAY(input , length , "%x");

    uint64_t new_length ;
    uchar_t *output = md4_padding(input , 8*length , &new_length  ) ;


    printf("output after padding : \n" );
    PRINT_ARRAY(output , new_length /8 , "%x");
    

    uchar_t *hash = malloc(sizeof(uchar_t)*16) ; 
    
    md4_hash(output , new_length/8 , hash) ;
    printf("final result  : \n" );
    PRINT_ARRAY_NOSPC(hash , 16 , "%02x") ; 




    /* printf("\t\tthe input : \n") ;

    PRINT_ARRAY(original_cipher , 64 , "%x") ; 
    
    TEST_ON_TEXT_ENCRYPTION(aes , original_cipher  , encrypted_cipher, 64 , original_key,AesKey ) ;
    
    printf("\t\tthe encryption : \n") ;
    PRINT_ARRAY(encrypted_cipher , 64 , "%x") ; 
    
    TEST_ON_TEXT_DECRYPTION(aes , encrypted_cipher  , decrypted_cipher,  64 , original_key,AesKey ) ;
    printf("\t\tthe decryption : \n") ;
    PRINT_ARRAY(decrypted_cipher , 64 , "%x") ; 
     */





     // aes_decrypt(encrypted_cipher , decrypted_cipher , 64 ,   ) ;

    // TEST_ON_IMAGE_ENCRYPTION(aes , "flowers.jpg" , original_key ,AesKey);
    
    // void *my_key = malloc(sizeof(AesKey)) ;  
    
    // aes_set_key(my_key , original_key) ; 
    // AesKey *my_aes_key = (void *) my_key ; 

    // aes_encrypt(original_cipher ,encrypted_cipher , 64 ,my_key );


    return 0 ; 
}





/* int main() {
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
 */

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
