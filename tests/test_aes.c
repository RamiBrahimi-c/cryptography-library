#include "aes.h"
#include "hash.h"
#include "../include/common/utils.h"

#include "test_utils.h"
#include "redpike.h"


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


    // uchar_t *input = generate_garbage_string(0x2b , 64) ;
    uchar_t input[] = "hello world!!!!!hello world!!!!!" ; 
    size_t input_len = 32  ; 
    uchar_t iv[] = {0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17, 0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f } ; 
    size_t iv_len = 16  ; 
    // uchar_t input[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5, 0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28, 
    // 0xf4, 0x43, 0xe3, 0xca, 0x4d, 0x62, 0xb5, 0x9a, 0xca, 0x84, 0xe9, 0x90, 0xca, 0xca, 0xf5, 0xc5, 
    // 0x2b, 0x09, 0x30, 0xda, 0xa2, 0x3d, 0xe9, 0x4c, 0xe8, 0x70, 0x17, 0xba, 0x2d, 0x84, 0x98, 0x8d, 
    // 0xdf, 0xc9, 0xc5, 0x8d, 0xb6, 0x7a, 0xad, 0xa6, 0x13, 0xc2, 0xdd, 0x08, 0x45, 0x79, 0x41, 0xa6  } ;
    uchar_t key[] = {0x0 , 0x1 , 0x2 , 0x3 , 0x4 , 0x5 , 0x6 , 0x7 , 0x8 , 0x9 , 0xa , 0xb , 0xc , 0xd , 0xe , 0xf    } ; 
    // uchar_t key[] = { 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b ,
                        // 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b }  ;
    size_t  key_len = 16  ; 
    uchar_t output[128]   ;
    uchar_t output2[128]  ;
    // memset(output , 0 , 8) ;
    
    // void *key_ptr = malloc(sizeof(DesKey)) ;
    // des_set_key(key_ptr , key);
    // des_encrypt(input , output , 8 , key_ptr) ; 
    
    // printing input : 
    printf("input:\n");
    PRINT_ARRAY_NOSPCLEN(input , input_len , "%.c");
    PRINT_ARRAY_NOSPCLEN(input , input_len , "%.2x");
    /*31323334353637383930616263646566313233343536373839306162636465663132333435363738393061626364656631323334353637383930616263646566*/
    /*b3de66418920957d41abf4a5b28e8f2dfab2a7fbade52c7184f0e3e68986d445f3d779b793b8c88c839a8ff964428aaadc0c59cf6c3cadfa262d4ed2*/
    // printing key : 
    printf("key:\n");
    PRINT_ARRAY_NOSPCLEN(key , key_len , "%.2x");
    
    // printing IV : 
    printf("iv :\n");
    PRINT_ARRAY_NOSPCLEN(iv , iv_len , "%.2x");
    
    // testing the encryption
    // TEST_ON_TEXT_ENCRYPTION(aes,input,output  ,input_len ,key  , key_len, AesKey) ;
    // TEST_ON_TEXT_ENCRYPTION_MODE(aes ,ctr ,input,output , iv ,input_len ,key  , key_len, AesKey) ;
    TEST_ON_TEXT_ENCRYPTION_MODE(aes ,ctr ,input,output , iv ,input_len ,key  , key_len, AesKey) ;
    
    // seeing the output : 
    printf("output ctr:\n");
    PRINT_ARRAY_NOSPCLEN(output , input_len , "%x");
    
    TEST_ON_TEXT_ENCRYPTION_MODE(aes ,ofb ,input,output , iv ,input_len ,key  , key_len, AesKey) ;
    printf("output ofb:\n");
    PRINT_ARRAY_NOSPCLEN(output , input_len , "%x");
    
    // do a decryption : 
    TEST_ON_TEXT_DECRYPTION(aes,output,output2 ,input_len,key  , key_len, AesKey) ;
    // TEST_ON_TEXT_DECRYPTION_MODE(aes , ctr,output,output2 , iv,input_len,key  , key_len, AesKey) ;
    
    // see the decrypted output : 
    printf("output decrypted:\n");
    PRINT_ARRAY_NOSPCLEN(output2 , input_len , "%c");
    



    return 0 ; 
}

