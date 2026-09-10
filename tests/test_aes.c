#include "aes.h"
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

    


    // uchar_t *input = generate_garbage_string(0x0 , 64) ;
    uchar_t input[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5, 0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28, 
                        0xf4, 0x43, 0xe3, 0xca, 0x4d, 0x62, 0xb5, 0x9a, 0xca, 0x84, 0xe9, 0x90, 0xca, 0xca, 0xf5, 0xc5, 
                        0x2b, 0x09, 0x30, 0xda, 0xa2, 0x3d, 0xe9, 0x4c, 0xe8, 0x70, 0x17, 0xba, 0x2d, 0x84, 0x98, 0x8d, 
                        0xdf, 0xc9, 0xc5, 0x8d, 0xb6, 0x7a, 0xad, 0xa6, 0x13, 0xc2, 0xdd, 0x08, 0x45, 0x79, 0x41, 0xa6  } ;
    uchar_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81 ,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }  ;
    size_t key_len = 16 ; 
    uchar_t output[64]  ;
    uchar_t output2[64]  ;
    // memset(output , 0 , 8) ;
    
    // void *key_ptr = malloc(sizeof(DesKey)) ;
    // des_set_key(key_ptr , key);
    // des_encrypt(input , output , 8 , key_ptr) ; 
    block_cipher_mode_operation = ECB ;
    printf("input:\n");
    PRINT_ARRAY_NOSPCLEN(input , 64 , "%.2x");
    printf("key:\n");
    PRINT_ARRAY_NOSPCLEN(key , key_len , "%.2x");
    
    TEST_ON_TEXT_ENCRYPTION(aes,input,output,64,key  , key_len, AesKey) ;
    
    printf("output:\n");
    PRINT_ARRAY_NAI(output , 64 , "%.2x");
    
    
    TEST_ON_TEXT_DECRYPTION(aes,output,output2,64,key  , key_len, AesKey) ;
    
    
    printf("output decrypted:\n");
    PRINT_ARRAY_NAI(output2 , 64 , "%.2x");
    
    // block_cipher_mode_operation = CBC ;
        // TEST_ON_IMAGE_ENCRYPTION(aes , "rami.png" , key , key_len , AesKey) ; 
    // des_decrypt(output , output2 , 8 , key_ptr) ; 
    // printf("output2:\n");
    // PRINT_ARRAY_NAI(output , 8 , "%.2x");
    
    return 0 ; 
}

