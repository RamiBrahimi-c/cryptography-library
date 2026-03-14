/* #include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/ciphers/substitution.h"


#define PRINT_ARRAY(array , length) \
    do { \
        printf("Array (%d):\n", length); \
        for (int _i = 0; _i < (length); _i++) { \
                printf("%1d ", (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)





int main() {



    uchar_t *original_text =(uchar_t*) "HELLOWORLDDDDDD" ; 
    uchar_t encrypted_text[1024]  ; 


    char *key_text = "ZYXWVUTSRQPONMLKJIHGFEDCBA" ; 
    
    void *substitution_hill = calloc(1 , sizeof(SubstitutionKey)) ; 
    assert(substitution_hill != NULL);

    SubstitutionKey *keyyy = (SubstitutionKey*) substitution_hill ; 
    

    substittution_set_key(substitution_hill , key_text);
    printf("after setting key\n");
    printf("length of map %u \n" ,keyyy->alphabet_size );

    
    printf("text to encrypt : %s\nlength : %d \n" , original_text , strlen(original_text)) ; 
    PRINT_ARRAY(original_text , strlen(original_text));
    
    substittution_encrypt(original_text ,encrypted_text , substitution_hill );
    
    
    printf("encrypted text : %s\n" , encrypted_text) ; 
    PRINT_ARRAY(encrypted_text , strlen(encrypted_text));
    
    uchar_t decrypted_text[1024] ; 
    
    substittution_decrypt(encrypted_text , decrypted_text , substitution_hill);
    
    printf("decrypted text : %s\n" , decrypted_text) ; 

    printf("diff %d \n" , strcmp(original_text , decrypted_text));



} */