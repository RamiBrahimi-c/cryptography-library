/* #include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// #include "../include/ciphers/affine.h"


int main()  {

    __u_char *original_text =(__u_char*) "hello everyone" ; 
    __u_char encrypted_text[1024]  ; 


    char *key_text = "19" ; 
    
    void *key_affine = calloc(1 , sizeof(AffineKey)) ; 
    assert(key_affine != NULL);


    affine_set_key(key_affine , key_text);
    printf("key.a = %lu key.b=%lu \n" , ((AffineKey *) *(&key_affine))->a , ((AffineKey *) *(&key_affine))->b) ; 
    
    affine_encrypt(original_text ,encrypted_text , key_affine );

    printf("text to encrypt : %s\n" , original_text) ; 
    
    
    printf("encrypted text : %s\n" , encrypted_text) ; 
    
    __u_char decrypted_text[1024] ; 
    
    affine_decrypt(encrypted_text , decrypted_text , key_affine);
    
    printf("decrypted text : %s\n" , decrypted_text) ; 


}


 */