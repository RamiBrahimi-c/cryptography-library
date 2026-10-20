#include "affine.h"
#include "hill.h"
#include "substitution.h"
#include "rc4.h"
#include "des.h"
#include "3des.h"
#include "aes.h"
#include "rsa.h"
#include "elgamal.h"
#include "dh.h"
#include "hash.h"
#include "../include/common/utils.h"

#include "test_utils.h"
#include "redpike.h"


#define STB_IMAGE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image_write.h"


#define STB_VORBIS_IMPLEMENTATION
#include "third-party/stb-nothing/stb_vorbis.c"



#include "aes.h"
#include "tea.h"
#include "xtea.h"
#include "blowfish.h"
#include "hash.h"






int main(int argc , char *argv[]) {

    // :catwiggle:

    // parse(argv) ; 
    uchar_t plaintext[16] = {0}  ;
    size_t plaintext_len = 16 ; 
    uchar_t key_str[32] = {0}  ;
    size_t key_str_len = 16 ; 
    uchar_t iv[] = {0xfe , 0xe1 , 0xde , 0xad , 0xc0 , 0xff , 0xee , 0x67}  ;
    size_t iv_len = 8 ; 
    uchar_t *encrypted_text = malloc(sizeof(uchar_t)*plaintext_len) ; 
    if (encrypted_text == NULL) {
        abort() ; 
    }
    uchar_t *decrypted_text = malloc(sizeof(uchar_t)*plaintext_len) ; 
    if (decrypted_text == NULL) {
        abort() ; 
    }

    key_str_len = 16 ; 
    DEMO_ON_TEXT_ENCRYPTION(aes , plaintext , encrypted_text , plaintext_len, key_str , key_str_len      , AesKey ) ;
    DEMO_ON_TEXT_DECRYPTION(aes , encrypted_text , decrypted_text , plaintext_len, key_str , key_str_len , AesKey ) ;
    
    key_str_len = 8 ; 
    key_str[0] = 0x89 ; 
    key_str[16] = 0xaa ; 
    plaintext[0] = 0xca ; 

    DEMO_ON_TEXT_ENCRYPTION(des , plaintext , encrypted_text , plaintext_len, key_str , key_str_len , DesKey ) ;
    key_str_len = 24 ; 
    DEMO_ON_TEXT_ENCRYPTION(tdes , plaintext , encrypted_text , plaintext_len, key_str , key_str_len , TDesKey ) ;
    
    key_str[0] = 0x0 ; 
    
    key_str_len = 16 ; 
    DEMO_ON_TEXT_ENCRYPTION(rc4 , plaintext , encrypted_text , plaintext_len, key_str , key_str_len , Rc4Key ) ;
    
    key_str_len = 16 ; 
    DEMO_ON_TEXT_ENCRYPTION(blowfish , plaintext , encrypted_text , plaintext_len, key_str , key_str_len , BlowfishKey ) ;
    
    key_str_len = 16 ; 
    DEMO_ON_TEXT_ENCRYPTION(tea , plaintext , encrypted_text , plaintext_len, key_str , key_str_len , TeaKey ) ;

    return 0 ; 
}

