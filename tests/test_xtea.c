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


int main() {

    // :catwiggle:


    // uchar_t *input = generate_garbage_string(0x2b , 64) ;
    uchar_t input[] = {'1' , '2' , '3' , '4' , '5'   , '6' , '7' , '8' , '9' , '0' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f'    , 
                         '1' , '2' , '3' , '4' , '5'   , '6' , '7' , '8' , '9' , '0' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f'  , 
                         '1' , '2' , '3' , '4' , '5'   , '6' , '7' , '8' , '9' , '0' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f'  , 
                         '1' , '2' , '3' , '4' , '5'   , '6' , '7' , '8' , '9' , '0' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' }  ; 
    size_t input_len = 16 * 4 ; 
    
    uchar_t iv[] = {0x12 , 0x12  , 0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 ,0x12 , 0x12 } ; 
    size_t iv_len = 16  ; 
    
    
    uchar_t key[] = { 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b ,
                        0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b }  ;
    size_t  key_len = 16  ; 
    
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
    // TEST_ON_TEXT_ENCRYPTION(xtea,input,output  ,input_len ,key  , key_len, XTeaKey ) ;
    TEST_ON_TEXT_ENCRYPTION_MODE(xtea ,cbc ,input,output , iv ,input_len ,key  , key_len, XTeaKey) ;
    
    // seeing the output : 
    printf("output:\n");
    PRINT_ARRAY_NOSPCLEN(output , input_len , "%x");
    
    // do a decryption : 
    // TEST_ON_TEXT_DECRYPTION(xtea ,output,output2 ,input_len,key  , key_len, XTeaKey) ;
    TEST_ON_TEXT_DECRYPTION_MODE(xtea , cbc ,output,output2 , iv,input_len,key  , key_len, XTeaKey) ;
    
    // see the decrypted output : 
    printf("output decrypted:\n");
    PRINT_ARRAY_NOSPCLEN(output2 , input_len , "%c");
    



    return 0 ; 
}

