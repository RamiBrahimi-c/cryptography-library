/* #include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/ciphers/hill.h"

#define PRINT_ARRAY(array , length) \
    do { \
        printf("Array (%d):\n", length); \
        for (int _i = 0; _i < (length); _i++) { \
                printf("%1d ", (array)[_i]); \
        } \
        printf("\n"); \
    } while (0)



#include <time.h>

int main() {

    srand(time(0));

    __u_char *original_text =(__u_char*) "hi hi hi main.c is there hahahahah!!!" ; 
    __u_char encrypted_text[1024]  ; 


    char *key_text = "2" ; 
    
    void *key_hill = calloc(1 , sizeof(HillKey)) ; 
    assert(key_hill != NULL);

    HillKey *keyyy = (HillKey*) key_hill ; 
    

    hill_set_key(key_hill , key_text);
    printf("after setting key\n");
    printf("degree of matrix %u \n" ,keyyy->n );

    for (size_t i = 0; i < keyyy->n; i++)
    {
        for (size_t j = 0; j < keyyy->n; j++)
        {
            printf(" %d " , keyyy->matrix[i][j]);
            
        }
        printf("\n");
    }
    
    // exit(0);
    // printf("key.a = %lu key.b=%lu \n" , ((HillKey *) *(&key_hill))->a , ((HillKey *) *(&key_hill))->b) ; 
    
    printf("text to encrypt : %s\nlength : %d \n" , original_text , strlen(original_text)) ; 
    PRINT_ARRAY(original_text , strlen(original_text));
    
    hill_encrypt(original_text ,encrypted_text , key_hill );
    
    
    printf("encrypted text : %s\n" , encrypted_text) ; 
    PRINT_ARRAY(encrypted_text , strlen(encrypted_text));
    
    __u_char decrypted_text[1024] ; 
    
    hill_decrypt(encrypted_text , decrypted_text , key_hill);
    
    printf("decrypted text : %s\n" , decrypted_text) ; 

    printf("diff %d \n" , strcmp(original_text , decrypted_text));
}
 */