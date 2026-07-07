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
#include "redpike.h"


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
#include "tea.h"
#include "xtea.h"
#include "blowfish.h"
#include "hash.h"



#define TEST_HASH_FUNCTION(name , data , length , digest)\
    printf("data : \n")\
    PRINT_ARRAY_NAI(data , length)\
    name##hash(data , length , digest);\



int main() {

    // :catwiggle:

    
    // uchar_t key[] = "4566546546546545645sd5qs4d5sq4dqs54dQS4dQSQSDQs54dQdQdQqs555165165161" ; 
    // size_t key_len = 69 ; 

    // uchar_t data[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ; 
    // size_t data_len = 75 ; 

    // uchar_t mac[32] ; 
    // size_t mac_len = 32 ; 

    // printf("key : \n" );
    // PRINT_ARRAY_NAI(key , key_len , "%0.2x") ;
    // PRINT_ARRAY_NOSPCLEN(key , key_len , "%0.2x") ;


    // printf("data : \n" );
    // PRINT_ARRAY_NAI(data , data_len , "%0.2x") ;
    // PRINT_ARRAY_NOSPCLEN(data , data_len , "%0.2x") ;


    
    // sha512_hash(input , length , digest) ; 
    // sha256_hash(input , length , digest) ; 
    // hmac_sha256(key , key_len , data , data_len , mac) ; 
    // hmac_sha512(key , key_len , data , data_len , mac) ; 
    size_t salt_len = 10;
    uchar_t salt[] = { 0x8e , 0x94 , 0xef , 0x80 , 0x5b , 0x93 , 0xe6 , 0x83 , 0xff , 0x18};
    size_t ikm_len  = 5;
    uchar_t ikm[]  = "hello";
    size_t info_len = 3 ;
    uchar_t info[] = { 0x12 , 0x34 , 0x56 };
    size_t okm_len = 32;
    uchar_t okm[32] ;
    
    hkdf_sha256(salt ,salt_len , ikm , ikm_len , info , info_len , okm , okm_len ) ; 
    
    
    printf("okm : \n" );
    PRINT_ARRAY_NAI(okm , okm_len , "%0.2x") ;
    PRINT_ARRAY_NOSPCLEN(okm , okm_len , "%0.2x") ;
    
    // hmac_sha256mine(key , key_len , data , data_len , mac) ; 
    // hmac_sha256mine(key , key_len , data , data_len , mac) ; 
    // hkdf_sha256_mine(salt ,salt_len , ikm , ikm_len , info , info_len , okm , okm_len ) ; 

    printf("okm mine : \n" );
    PRINT_ARRAY_NAI(okm , okm_len , "%0.2x") ;
    PRINT_ARRAY_NOSPCLEN(okm , okm_len , "%0.2x") ;


    printf("!!\n") ; 

    return 0 ; 
}

