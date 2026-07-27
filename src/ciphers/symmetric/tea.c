#include "tea.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
    thanks to : https://link.springer.com/chapter/10.1007/3-540-60590-8_29
*/


/*
    uchar_t *vv : gotta be size of 8 bytes
    uchar_t *kk : gotta be size of 16 bytes
*/

static void tea_encrypt_block(uchar_t *vv , uchar_t *kk) {

    // printf("") ; 
    // the specifications has `long *` but long could be 8 bytes so 
    // imma limit it to exactly 32 bit word
    uint32_t v[2] ;
    uint32_t k[4] ;

    // set up `v` and `k`
  v[0] = (vv[0] << 24) | (vv[1] << 16) | (vv[2] << 8) | (vv[3]) ; 
  v[1] = (vv[4] << 24) | (vv[5] << 16) | (vv[6] << 8) | (vv[7]) ; 



  k[0] = (kk[0] << 24) | (kk[1] << 16) | (kk[2] << 8) | (kk[3]) ; 
  k[1] = (kk[4] << 24) | (kk[5] << 16) | (kk[6] << 8) | (kk[7]) ; 
  k[2] = (kk[8] << 24) | (kk[9] << 16) | (kk[10] << 8) | (kk[11]) ; 
  k[3] = (kk[12] << 24) | (kk[13] << 16) | (kk[14] << 8) | (kk[15]) ; 

    // the algorithm starts from here

    uint32_t y = v[0] , z = v[1] , sum = 0;
    uint32_t delta = 0x9e3779b9; // (golden ratio -1 ) * 2^32
    int n = 32; // advised by authors to do 32 cycle 

    while (n > 0)
    {
        sum += delta ;
        y += ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]) ; 
        z += ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]) ;
        
        n-- ; 
    }
    v[0] = y ; 
    v[1] = z ; 


    // copy back to vv ...
  vv[0] = v[0] >> 24 ; 
  vv[1] = v[0] >> 16 ; 
  vv[2] = v[0] >> 8 ; 
  vv[3] = v[0]  ; 


  vv[4] = v[1] >> 24 ; 
  vv[5] = v[1] >> 16 ; 
  vv[6] = v[1] >> 8 ; 
  vv[7] = v[1]  ; 


}


static 
/*
    uchar_t *vv : gotta be size of 8 bytes
    uchar_t *kk : gotta be size of 16 bytes
*/
void tea_decrypt_block(uchar_t *vv , uchar_t *kk) {

    // printf("") ; 
    // the specifications has `long *` but long could be 8 bytes so 
    // imma limit it to exactly 32 bit word
    uint32_t v[2] ;
    uint32_t k[4] ;

    // set up `v` and `k`
  v[0] = (vv[0] << 24) | (vv[1] << 16) | (vv[2] << 8) | (vv[3]) ; 
  v[1] = (vv[4] << 24) | (vv[5] << 16) | (vv[6] << 8) | (vv[7]) ; 



  k[0] = (kk[0] << 24) | (kk[1] << 16) | (kk[2] << 8) | (kk[3]) ; 
  k[1] = (kk[4] << 24) | (kk[5] << 16) | (kk[6] << 8) | (kk[7]) ; 
  k[2] = (kk[8] << 24) | (kk[9] << 16) | (kk[10] << 8) | (kk[11]) ; 
  k[3] = (kk[12] << 24) | (kk[13] << 16) | (kk[14] << 8) | (kk[15]) ; 

    // the algorithm starts from here

    uint32_t y = v[0] , z = v[1] , sum ;
    uint32_t delta = 0x9e3779b9;
    sum = delta << 5 ; // (golden ratio -1 ) * 2^64 and took lower 32bits ig
    int n = 32;

    while (n > 0)
    {
        z -= ((y<<4)+k[2]) ^ (y+sum) ^ ((y>>5)+k[3]) ;
        y -= ((z<<4)+k[0]) ^ (z+sum) ^ ((z>>5)+k[1]) ; 
        
        sum -= delta ;
        n-- ; 
    }
    v[0] = y ; 
    v[1] = z ; 


    // copy back to vv ...
  vv[0] = v[0] >> 24 ; 
  vv[1] = v[0] >> 16 ; 
  vv[2] = v[0] >> 8 ; 
  vv[3] = v[0]  ; 


  vv[4] = v[1] >> 24 ; 
  vv[5] = v[1] >> 16 ; 
  vv[6] = v[1] >> 8 ; 
  vv[7] = v[1]  ; 


}



void tea_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {
    assert(key != NULL && "key is null");
    TeaKey *tea_key = (TeaKey *) (key) ;

    // meh just basic ECB
    // TODO : well ECB aint that good gotta add other modes ...
    for (size_t i = 0; i < length/8; i++)
    {
        tea_encrypt_block(input + i*8 , tea_key->key) ; 
    }
    
    // whatever
    memcpy(output , input , sizeof(uchar_t)*length) ; 
    
    
}


void tea_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key) {
    assert(key != NULL && "key is null");
    TeaKey *tea_key = (TeaKey *) (key) ;
    
    // TODO : add other modes here too ...
    for (size_t i = 0; i < length/8; i++)
    {
        tea_decrypt_block(input + i*8 , tea_key->key) ; 
    }
    
    memcpy(output , input , sizeof(uchar_t)*length) ; 
    
}


void tea_set_key(void* key_struct, const uchar_t* key_str , size_t key_len) {
    TeaKey *tea_key = (TeaKey *) key_struct ;
    
    // how can we make sure that key_str is actually TEA_KEY_MAX_SIZE bytes ...
    assert(key_len >= 16 && "key length here must be 16 bytes");    
    memcpy(tea_key->key , key_str , sizeof(uchar_t)*TEA_KEY_MAX_SIZE) ; 
    tea_key->length = TEA_KEY_MAX_SIZE   ; 
    tea_key->type = BLOCK_CIPHER ; 
}


void tea_free_key(void* key_struct);

Cipher* get_tea_cipher(void);




