#include "aes.h"
#include "hash.h"
#include "../include/common/utils.h"

#include "test_utils.h"
#include "redpike.h"


#include "des.h"
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


    // uchar_t *input = generate_garbage_string(0x2b , 64) ;
    // uchar_t input[] = { 0x4E , 0x65 , 0x74 , 0x73 , 0x63 , 0x61 , 0x70 , 0x65}  ; 
    uchar_t input[] = { 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00}  ; 
    size_t input_len = 8 ; 
    uchar_t iv[] = {0x12 , 0x12  , 0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 } ; 
    size_t iv_len = 16  ; 
    // uchar_t input[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5, 0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28, 
                        // 0xf4, 0x43, 0xe3, 0xca, 0x4d, 0x62, 0xb5, 0x9a, 0xca, 0x84, 0xe9, 0x90, 0xca, 0xca, 0xf5, 0xc5, 
                        // 0x2b, 0x09, 0x30, 0xda, 0xa2, 0x3d, 0xe9, 0x4c, 0xe8, 0x70, 0x17, 0xba, 0x2d, 0x84, 0x98, 0x8d, 
                        // 0xdf, 0xc9, 0xc5, 0x8d, 0xb6, 0x7a, 0xad, 0xa6, 0x13, 0xc2, 0xdd, 0x08, 0x45, 0x79, 0x41, 0xa6  } ;
    uchar_t key[] = { 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 } ; 
    size_t  key_len = 8  ; 

    uchar_t output[128]   ;
    uchar_t output2[128]  ;
    

    // printing input : 
    printf("input:\n");
    PRINT_ARRAY_NOSPCLEN(input , input_len , "%.c");
    PRINT_ARRAY_NOSPCLEN(input , input_len , "%.2x");
   
   
    
    // printing key : 
    printf("key:\n");
    PRINT_ARRAY_NOSPCLEN(key , key_len , "%.2x");
    
    // printing IV : 
    printf("iv :\n");
    PRINT_ARRAY_NOSPCLEN(iv , iv_len , "%.2x");
    
    // testing the encryption
    TEST_ON_TEXT_ENCRYPTION(des,input,output  ,input_len ,key  , key_len, DesKey ) ;
    // TEST_ON_TEXT_ENCRYPTION_MODE(des ,ctr ,input,output , iv ,input_len ,key  , key_len, DesKey) ;
    
    // seeing the output : 
    printf("output:\n");
    PRINT_ARRAY_NOSPCLEN(output , input_len , "%x");
    
    // do a decryption : 
    TEST_ON_TEXT_DECRYPTION(des ,output,output2 ,input_len,key  , key_len, DesKey) ;
    // TEST_ON_TEXT_DECRYPTION_MODE(des , ctr ,output,output2 , iv,input_len,key  , key_len, DesKey) ;
    
    // see the decrypted output : 
    printf("output decrypted:\n");
    PRINT_ARRAY_NOSPCLEN(output2 , input_len , "%c");
    for (int i = 0 ; i < input_len  ; i++) {
        printf(" %x " ,output2[i]  ) ; 
    }printf("\n") ; 



    return 0 ; 
}

