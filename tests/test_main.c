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



int main() {

    // :catwiggle:


    uchar_t digest[64] ;
    uint64_t length = 690 ;
    uchar_t input[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ; 

    printf("input : \n" );
    PRINT_ARRAY(input , length , "%c");
    PRINT_ARRAY(input , length , "%x");
    
    uint64_t a = 0x0123456789abcdef ; 
    uint64_t b ; 
    uchar_t temp[8] ;
    uchar_t temp2[8] = {0x67 , 0x89 , 0xab , 0xcd  , 0x01 , 0x23 , 0x45 ,  0xef} ;

    
    sha512_hash(input , length , digest) ; 

    printf("digest : \n" );
    PRINT_ARRAY(digest , 64 , "%0.2x") ;
    PRINT_ARRAY_NOSPC(digest , 64 , "%0.2x") ;




    return 0 ; 
}

