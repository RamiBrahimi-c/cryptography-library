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


    size_t salt_len = 10;
    uchar_t salt[] = { 0x8e , 0x94 , 0xef , 0x80 , 0x5b , 0x93 , 0xe6 , 0x83 , 0xff , 0x18};
    size_t ikm_len  = 76;
    uchar_t ikm[]  = "hellolkqnsklqsndqsndlqskndqskjndqskjndhellolkqnsklqsndqsndlqskndqskjndqskjnd";
    size_t info_len = 3 ;
    uchar_t info[] = { 0x12 , 0x34 , 0x56 };
    size_t okm_len = 64;
    uchar_t okm[64] ;



    // hkdf_sha256(salt ,salt_len , ikm , ikm_len , info , info_len , okm , okm_len ) ; 
    hkdf_sha512(salt ,salt_len , ikm , ikm_len , info , info_len , okm , okm_len ) ; 
    
    
    printf("okm : \n" );
    PRINT_ARRAY_NAI(okm , okm_len , "%0.2x") ;
    PRINT_ARRAY_NOSPCLEN(okm , okm_len , "%0.2x") ;
    

    return 0 ; 
}

