// #include "rc4.h"
// // #include "../include/ciphers/symmetric/rc4.h"
// #include "test_utils.h"

// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <assert.h>


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"



// typedef unsigned char uchar_t ;



// // TEST(rc4_encrypt_basic) {
// //     char output[100];
// //     Rc4Key key ;  // a=5, b=8
    
// //     // affine_encrypt("HELLO", output , 5, &key);
// //     // ASSERT_STR_EQ(output, "RCLLA");
// //     return 0;
// // }

// // TEST(affine_invalid_key) {
// //     Rc4Key key ;  // a=2 (invalid - not coprime to 26)
// //     // Should handle error case
// //     ASSERT_TRUE(is_valid_affine_key(&key) == 0);
// //     return 0;
// // }


// static void printInfosRc4Key(Rc4Key rc4_key) {
//     printf("======= printing info Rc4 Key =======\n");
//     printf("length : %d \n" , rc4_key.length);
//     for (size_t i = 0; i < rc4_key.length; i++)
//     {
//         printf("[%ld] : %d \n" , i , rc4_key.state[i] );
//     }
    
// }


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

// #define FULL_PATH_LENGTH 64







// int main() {
//     // RUN_TEST(affine_encrypt);
//     // RUN_TEST(affine_invalid_key);





//     printf("INFO : RUNNING TEST RC4 \n");


//     int width, height, channels;
//     char *filename = "coast.jpg" ; 
//     char *directory_input_images = "tests/img" ; 
//     char *directory_output_images = "tests/results" ; 
//     char *algo_name = "rc4" ; 
//     char *enc_type_algo = "enc" ; 
//     char *dec_type_algo = "dec" ; 
//     char full_path_image_file[FULL_PATH_LENGTH]  ; 
//     char full_path_result_image_file[FULL_PATH_LENGTH]  ; 



//     setupFullImagePath(directory_input_images, filename ,full_path_image_file  , FULL_PATH_LENGTH ) ; 
//     printf("%s\n" , full_path_image_file) ; 
//     setupFullResultImagePath(directory_output_images , enc_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH) ; 
//     printf("%s\n" , full_path_result_image_file) ; 

//     __u_char *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0); ; 
    
//     assert(original_text != NULL && "original text is null") ; 
//     printf("Image: %dx%d pixels, %d channels per pixel\n", width, height, channels);
//     printf("Total bytes: %d\n", width * height * channels);
//     printf("length : %d \n" , strlen(original_text));

    

//     __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    
//     assert(encrypted_text != NULL && "failed to allocate") ; 
    


//     char *key_text = "Key" ; 
    
//     void *key = calloc(1 , sizeof(Rc4Key)) ; 
//     assert(key != NULL);
    
//     Rc4Key *key_rc4 = (Rc4Key *) key ; 

//     printf("INFO: setting key... \n");

//     rc4_set_key(key_rc4 , key_text);

//     printf("INFO: key set with success\n");
    
//     printf("key : %s \n" ,key_text ) ; 
//     // printInfosRc4Key(*key_rc4) ; 


//     // char *original_text = "hello everyone" ; 
//     // char encrypted_text[2048] ; 
//     // int len =  strlen(original_text) ;
//     int len =  width*height*channels ;
    
//     // printf("text to encrypt : %s\n" , original_text) ; 
//     // printf("i=%d\tj=%d" , key_rc4->i , key_rc4->j) ; 
//     rc4_encrypt(original_text , encrypted_text ,len , key) ; 
//     // rc4_encrypt(original_text , encrypted_text ,width*height*channels , key) ; 
    
//     // printf("encrypted text  : %s \n" , encrypted_text) ; 
    
    
//     stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels);
    
//     printf("INFO: saved to %s \n" , full_path_result_image_file);


//     // stbi_image_free(original_text);
    
//     printf("Done! Check result in %s \n" , full_path_result_image_file );
    
    
//     // setupFullResultImagePath(directory_output_images , dec_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH) ; 
//     // printf("%s\n" , full_path_result_image_file) ; 
    
    
//     // char *decrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    
//     // assert(encrypted_text != NULL && "failed to allocate") ; 

//     // // rc4_set_key(key , "KeyKey") ; 
//     // rc4_encrypt( encrypted_text , decrypted_text ,len , key) ; 
   


//     // stbi_write_png(full_path_result_image_file, width, height, channels, decrypted_text, width * channels);
    
//     // printf("INFO: saved to %s \n" , full_path_result_image_file);


//     stbi_image_free(original_text);
    
//     printf("Done! Check result in %s \n" , full_path_result_image_file );
        
   
//     // // rc4_decrypt( encrypted_text , decrypted_text ,len , key) ; 
    
//     // printf("decrypted text : %s\n" , decrypted_text) ; 
    
//     // char *decrypted_text[2048] ; 
//     // // rc4_encrypt( encrypted_text , decrypted_text ,len , key) ; 
//     // printf("i=%d\tj=%d\n" , key_rc4->i , key_rc4->j) ; 
//     // rc4_set_key(key , key_text) ; 
//     // rc4_encrypt( encrypted_text , decrypted_text ,len , key) ; 
//     // // rc4_decrypt( encrypted_text , decrypted_text ,len , key) ; 
    
//     // printf("decrypted text : %s\n" , decrypted_text) ; 


//     return 0;
// }
