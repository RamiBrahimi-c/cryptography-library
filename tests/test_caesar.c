/* #include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/ciphers/caesar.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


typedef unsigned char uchar_t ;


int main()  {

    int width, height, channels;


    __u_char *original_text = stbi_load("./tests/img/mall.jpg", &width, &height, &channels, 0); ; 
    assert(original_text != NULL && "original text is null") ; 


    printf("Image: %dx%d pixels, %d channels per pixel\n", width, height, channels);
    printf("Total bytes: %d\n", width * height * channels);
    printf("length : %d \n" , strlen(original_text));


    __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    assert(encrypted_text != NULL && "failed to allocate ") ; 


    char *key_text = "19" ; 
    
    // void *key_affine = calloc(1 , sizeof(AffineKey)) ; 
    // assert(key_affine != NULL);

    // printf("INFO: setting key... \n");

    // affine_set_key(key_affine , key_text);
    // printf("key.a = %lu key.b=%lu \n" , ((AffineKey *) *(&key_affine))->a , ((AffineKey *) *(&key_affine))->b) ; 
    printf("INFO: key set with success\n");
    
    encrypt_caesar(original_text , width * height * channels, encrypted_text   );

    printf("INFO: encrypted with success\n");

        char *output_file = "ceaser.png" ; 


    stbi_write_png(output_file, width, height, channels, encrypted_text, width * channels);

    printf("INFO: saved to %s\n" , output_file);


    // stbi_image_free(original_text);
    
    printf("Done! Check %s\n" , output_file);
    
    // printf("text to encrypt : %s\n" , original_text) ; 
    
    
    // printf("encrypted text : %s\n" , encrypted_text) ; 
    
    // __u_char *decrypted_text = malloc(sizeof(uchar_t) * width * height * channels) ; 
    
    // affine_decrypt(encrypted_text , decrypted_text , key_affine);
    
    // assert(encrypted_text != NULL && "failed to allocate ") ;    
    // printf("decrypted text : %s\n" , decrypted_text) ; 

    // printf("INFO: decrypted with success\n");


    // stbi_write_png("output4_dec.png", width, height, channels, decrypted_text, width * channels);

    // printf("INFO: saved to photo\n");


    stbi_image_free(original_text);
    
    
}


 */