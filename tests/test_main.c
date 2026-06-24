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



uchar_t* initilize_8bytes_block(uint64_t number) {
    return (uchar_t []) {number << 56 , number << 48 , number <<40 , number << 32 , number << 24 , number << 16 , number << 8 , number} ; 
}

int main() {


    uchar_t input[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} ; 
    uchar_t key[] = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 , 0x0} ; 

        
    printf("input : \n") ; 
    PRINT_ARRAY(input , 8 , "%x") ; 
    printf("key : \n") ; 
    PRINT_ARRAY(key , 8 , "%x") ; 
    
    
    void *blow_key = calloc(1 , sizeof(BlowfishKey)) ; 
    ASSERT_NOT_NULL(blow_key) ; 

    BlowfishKey *blowfish_key = (BlowfishKey *) blow_key ; 
    
    ASSERT_NOT_NULL(blowfish_key) ; 
    
    printf("blow_key pointer : %p \n" , blow_key) ; 
    printf("blowfish_key pointer : %p \n" , blowfish_key) ; 
    blowfish_set_key(   blowfish_key , key) ; 
    
    printf("key set up \n") ; 
    
    blowfish_encrypt(input , input , 8 , blowfish_key) ; 
    printf("encryption done !!! \n") ; 

    PRINT_ARRAY(input , 8 , "%02x") ; 
    PRINT_ARRAY_NOSPC(input , 8 , "%02x") ; 


   

    return 0 ; 
}

