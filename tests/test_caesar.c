//  #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <assert.h>
// #include "caesar.h"


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"


// typedef unsigned char uchar_t ;




// void strip_extension(char *filename) {
//     // Find the last dot in the filename
//     char *dot = strrchr(filename, '.');
    
//     // If a dot exists and it's not the first character (e.g., ".gitignore")
//     if (dot != NULL && dot != filename) {
//         *dot = '\0'; // Replace dot with null terminator
//     }
// }


// void setupFullImagePath(char *directory_name , char *filename , char *full_path , int total_size  ) {
//     snprintf(full_path ,total_size, "./%s/%s" , directory_name , filename )  ; 
// }

// void setupFullResultImagePath(char *directory_name , char *type_algo  , char *filename , char *algo_name , char *full_path ,  int total_size ) {
    
//     // NOTE: just to force the output to be .png
//     // NOTE: maybe add full date and time
//     char filename_copy[64] ; 
//     strcpy(filename_copy , filename) ; 

//     strip_extension(filename_copy) ;     
//     strcat(filename_copy , ".png") ; 
    
//     snprintf(full_path , total_size , "./%s/%s_%s_%s" , directory_name , type_algo   , algo_name, filename_copy )  ; 

// }

// #define FULL_PATH_LENGTH 256





// int main()  {

//     int width, height, channels;
//     char *filename = "flowers.jpg" ; 
//     char *directory_input_images = "tests/img" ; 
//     char *directory_output_images = "tests/results" ; 
//     char *algo_name = "caesar" ; 
//     char *enc_type_algo = "enc" ; 
//     char *dec_type_algo = "dec" ; 
//     char full_path_image_file[FULL_PATH_LENGTH]  ; 
//     char full_path_result_image_file[FULL_PATH_LENGTH]  ; 

//     setupFullImagePath(directory_input_images, filename ,full_path_image_file  , FULL_PATH_LENGTH ) ; 
//     printf("%s\n" , full_path_image_file) ; 
//     setupFullResultImagePath(directory_output_images , enc_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH) ; 
//     printf("%s\n" , full_path_result_image_file) ; 


//     __u_char *original_text = "ASSINET" ; 
//     // __u_char *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0); ; 
//     assert(original_text != NULL && "original text is null") ; 


//     // printf("Image: %dx%d pixels, %d channels per pixel\n", width, height, channels);
//     // printf("Total bytes: %d\n", width * height * channels);
//     // printf("length : %d \n" , strlen(original_text));


//     __u_char encrypted_text[1024]  ; 
//     // __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
//     assert(encrypted_text != NULL && "failed to allocate ") ; 


//     char *key_text = "19" ; 
    
//     // void *key_affine = calloc(1 , sizeof(AffineKey)) ; 
//     // assert(key_affine != NULL);

//     // printf("INFO: setting key... \n");

//     // affine_set_key(key_affine , key_text);
//     // printf("key.a = %lu key.b=%lu \n" , ((AffineKey *) *(&key_affine))->a , ((AffineKey *) *(&key_affine))->b) ; 
//     printf("INFO: key set with success\n");
    
//     // encrypt_caesar(original_text , width * height * channels, encrypted_text   );
//     encrypt_caesar(original_text , strlen(original_text), encrypted_text   );

//     printf("INFO: encrypted with success\n");



//     // stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels);

//     // printf("INFO: saved to %s\n" , full_path_result_image_file);


    
//     printf("Done! Check %s\n" , full_path_result_image_file);
    
//     // printf("text to encrypt : %s\n" , original_text) ; 
    
    
//     printf("original text : %s\n" , original_text) ; 
//     printf("encrypted text : %s\n" , encrypted_text) ; 
    
//     // __u_char *decrypted_text = malloc(sizeof(uchar_t) * width * height * channels) ; 
    
//     // affine_decrypt(encrypted_text , decrypted_text , key_affine);
    
//     // assert(encrypted_text != NULL && "failed to allocate ") ;    
//     // printf("decrypted text : %s\n" , decrypted_text) ; 

//     // printf("INFO: decrypted with success\n");


//     // stbi_write_png("output4_dec.png", width, height, channels, decrypted_text, width * channels);

//     // printf("INFO: saved to photo\n");


//     // stbi_image_free(original_text);
    
    
// }

