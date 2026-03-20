
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hill.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            // Use memmove to safely shift the remaining characters back
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}


void strip_extension(char *filename) {
    // Find the last dot in the filename
    char *dot = strrchr(filename, '.');
    
    // If a dot exists and it's not the first character (e.g., ".gitignore")
    if (dot != NULL && dot != filename) {
        *dot = '\0'; // Replace dot with null terminator
    }
}


void setupFullImagePath(char *directory_name , char *filename , char *full_path , int total_size  ) {
    snprintf(full_path ,total_size, "./%s/%s" , directory_name , filename )  ; 
}

void setupFullResultImagePath(char *directory_name , char *type_algo  , char *filename , char *algo_name , char *full_path ,  int total_size ) {
    
    // NOTE: just to force the output to be .png
    // NOTE: maybe add full date and time
    char filename_copy[64] ; 
    strcpy(filename_copy , filename) ; 

    strip_extension(filename_copy) ;     
    strcat(filename_copy , ".png") ; 
    
    snprintf(full_path , total_size , "./%s/%s_%s_%s" , directory_name , type_algo   , algo_name, filename_copy )  ; 

}

#define FULL_PATH_LENGTH 256


int main()  {
    srand(time(0));

    int width, height, channels;
    char *filename = "summersky_w.jpg" ; 
    char *directory_input_images = "tests/img" ; 
    char *directory_output_images = "tests/results" ; 
    char *algo_name = "hill" ; 
    char *enc_type_algo = "enc" ; 
    char *dec_type_algo = "dec" ; 
    char full_path_image_file[FULL_PATH_LENGTH]  ; 
    char full_path_result_image_file[FULL_PATH_LENGTH]  ; 

    setupFullImagePath(directory_input_images, filename ,full_path_image_file  , FULL_PATH_LENGTH ) ; 
    printf("%s\n" , full_path_image_file) ; 
    setupFullResultImagePath(directory_output_images , enc_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH) ; 
    printf("%s\n" , full_path_result_image_file) ; 

    __u_char *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0); ; 
    
    assert(original_text != NULL && "original text is null") ; 
    
    __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    
    assert(encrypted_text != NULL && "failed to allocate") ; 
    



    char *key_text = "3" ; 
    
    void *key_hill = calloc(1 , sizeof(HillKey)) ; 
    assert(key_hill != NULL);

    

    hill_set_key(key_hill , key_text);
    
    printf("INFO: key set up with success\n");



    hill_encrypt(original_text ,encrypted_text  , width * height * channels, key_hill );
    
    
    printf("INFO: encrypted with success\n");
    


    

    stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels);

    printf("INFO: saved to %s \n" , full_path_result_image_file);


    stbi_image_free(original_text);
    
    printf("Done! Check result in %s \n" , full_path_result_image_file );
    

} 
 

 
