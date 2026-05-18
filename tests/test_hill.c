// // #include <stdint.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <assert.h>
// #include "hill.h"


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"


// char *strremove(char *str, const char *sub) {
//     size_t len = strlen(sub);
//     if (len > 0) {
//         char *p = str;
//         while ((p = strstr(p, sub)) != NULL) {
//             // Use memmove to safely shift the remaining characters back
//             memmove(p, p + len, strlen(p + len) + 1);
//         }
//     }
//     return str;
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

// #define FULL_PATH_LENGTH 256

// #define STB_VORBIS_IMPLEMENTATION
// #include "stb_vorbis.c"

// #define STB_VORBIS_HEADER_ONLY
// #include "stb_vorbis.c"


// #define STB_VORBIS_IMPLEMENTATION
// #include "stb_vorbis.c"


// int main()  {
//     srand(time(0));

//     int channels, sample_rate;
//     short *audio_data;
    
//     // 1. LOAD audio file -> get raw samples
//     int samples = stb_vorbis_decode_filename(
//         "tests/sound2.ogg", 
//         &channels, 
//         &sample_rate, 
//         &audio_data
//     );
//     if (samples < 0)
//     {
//         printf("kjblkblkj\n");
//         return 1;
//     }
    
//     printf("Audio: %d samples, %d channels, %d Hz\n", 
//            samples, channels, sample_rate);
//     printf("Total shorts: %d\n", samples * channels);
    
//         printf("alphabet length : %d \n" , ALPHABET_LENGTH) ; 

//     unsigned char *byte_data = (unsigned char *)audio_data;

//     // audio_data is now an array of 16-bit values: 
//     // [L,R,L,R,L,R,...] for stereo
//     // [sample,sample,sample,...] for mono
//     int total_bytes = samples * channels * sizeof(short);
//     // 2. YOUR ENCRYPTION HERE!
//     int total_shorts = samples * channels;

//         printf("hi\n");
//         void *key_hill = malloc(sizeof(HillKey)  ) ; 
//         // void *key = malloc(sizeof(Rc4Key)  ) ; 
//         // rc4_set_key(key , "3") ; 
//         // affine_set_key(key , "3") ; 
//         // affine_set_key(key , "3") ; 
//     hill_set_key(key_hill , "3");

//     // for (int i = 0; i < total_bytes; i++) {
//     //     // audio_data[i] is a 16-bit signed integer (-32768 to 32767)
//     //     byte_data[i] = your_audio_encryption(audio_data[i]);
//     //     // byte_data[i] = your_audio_encryption(audio_data[i]);
//     // }
//     unsigned char *output = malloc(sizeof(char) * total_bytes) ;
//     // rc4_encrypt(audio_data , output , total_bytes , key) ;
//      printf("encrypting ...\n");
//      hill_encrypt(byte_data , output , total_bytes , key_hill) ; 
//      printf("after encrypting ...\n");

//     FILE *ptr = fopen("output_hill_sound2.raw" , "wb+") ; 


//     fwrite(output , sizeof(char) , total_bytes , ptr) ; 


//     // 3. SAVE (you'll need a library for this - stb doesn't write audio)
//     // Could use libsndfile, dr_wav, or just raw binary
    
//     // 4. CLEAN UP
//     free(audio_data);




//     // int width, height, channels;
//     // char *filename = "coast.jpg" ; 
//     // char *directory_input_images = "tests/img" ; 
//     // char *directory_output_images = "tests/results" ; 
//     // char *algo_name = "hill" ; 
//     // char *enc_type_algo = "enc" ; 
//     // char *dec_type_algo = "dec" ; 
//     // char full_path_image_file[FULL_PATH_LENGTH]  ; 
//     // char full_path_result_image_file[FULL_PATH_LENGTH]  ; 

//     // setupFullImagePath(directory_input_images, filename ,full_path_image_file  , FULL_PATH_LENGTH ) ; 
//     // printf("%s\n" , full_path_image_file) ; 
//     // setupFullResultImagePath(directory_output_images , enc_type_algo , filename , algo_name ,full_path_result_image_file  , FULL_PATH_LENGTH) ; 
//     // printf("%s\n" , full_path_result_image_file) ; 

//     // __u_char *original_text = stbi_load(full_path_image_file, &width, &height, &channels, 0); ; 
    
//     // assert(original_text != NULL && "original text is null") ; 
    
//     // __u_char *encrypted_text = malloc(sizeof(uchar_t) * width * height * channels)  ; 
    
//     // assert(encrypted_text != NULL && "failed to allocate") ; 
    



//     // char *key_text = "3" ; 
    
//     // void *key_hill = calloc(1 , sizeof(HillKey)) ; 
//     // assert(key_hill != NULL);

    

//     // hill_set_key(key_hill , key_text);
    
//     // printf("INFO: key set up with success\n");



//     // hill_encrypt(original_text ,encrypted_text  , width * height * channels, key_hill );
    
    
//     // printf("INFO: encrypted with success\n");
    


    

//     // stbi_write_png(full_path_result_image_file, width, height, channels, encrypted_text, width * channels);

//     // printf("INFO: saved to %s \n" , full_path_result_image_file);


//     // stbi_image_free(original_text);
    
//     // printf("Done! Check result in %s \n" , full_path_result_image_file );
    

// } 
 

 
