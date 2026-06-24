#ifndef BLOWFISH_C
#define BLOWFISH_C


#include <stdint.h>

#include "../../cipher_interface.h"
#include "../../common/constants.h"

typedef struct {
    uchar_t *key ; 
    uint32_t length ;
    uint32_t p[18] ; 
    uint32_t s1[256] ; 
    uint32_t s2[256] ; 
    uint32_t s3[256] ; 
    uint32_t s4[256] ; 



} BlowfishKey;

/*
    before implementing :
        it sounds cool .. lets try implementing it !!!!!!!
    after implementing :
        yea ... wtv 🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡🐡
*/

void blowfish_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void blowfish_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void blowfish_set_key(void* key_struct, const char* key_str);
void blowfish_free_key(void* key_struct);

Cipher* get_blowfish_cipher(void);





#endif