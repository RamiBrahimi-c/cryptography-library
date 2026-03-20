// #include "affine.h"
// #include "test_utils.h"

// TEST(affine_encrypt_basic) {
//     char output[100];
//     AffineKey key = {5, 8};  // a=5, b=8
    
//     affine_encrypt("HELLO", output , 5, &key);
//     ASSERT_STR_EQ(output, "RCLLA");
//     return 0;
// }

// TEST(affine_invalid_key) {
//     AffineKey key = {2, 3};  // a=2 (invalid - not coprime to 26)
//     // Should handle error case
//     ASSERT_TRUE(is_valid_affine_key(&key) == 0);
//     return 0;
// }

// int main() {
//     RUN_TEST(affine_encrypt);
//     // RUN_TEST(affine_invalid_key);
//     return 0;
// }

//  #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <assert.h>
// #include "affine.h"


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"



/* 

int main()  {

    int width, height, channels;


    __u_char *original_text = stbi_load("./tests/img/wheel.png", &width, &height, &channels, 0); ; 
    assert(original_text != NULL && "original text is null") ; 


    printf("Image: %dx%d pixels, %d channels per pixel\n", width, height, channels);
    printf("Total bytes: %d\n", width * height * channels);
    printf("length : %d \n" , strlen(original_text));


    __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    assert(encrypted_text != NULL && "failed to allocate ") ; 


    char *key_text = "19" ; 
    
    void *key_affine = calloc(1 , sizeof(AffineKey)) ; 
    assert(key_affine != NULL);

    printf("INFO: setting key... \n");

    affine_set_key(key_affine , key_text);
    printf("key.a = %lu key.b=%lu \n" , ((AffineKey *) *(&key_affine))->a , ((AffineKey *) *(&key_affine))->b) ; 
    printf("INFO: key set with success\n");
    
    affine_encrypt(original_text ,encrypted_text  , width * height * channels, key_affine );

    printf("INFO: encrypted with success\n");

        char *output_file = "wheel_affine.png" ; 


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

