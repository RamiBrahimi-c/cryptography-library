#include "affine.h"
#include "hill.h"
#include "substitution.h"
#include "rc4.h"
#include "des.h"
#include "aes.h"
#include "rsa.h"
#include "tea.h"
#include "xtea.h"
#include "redpike.h"
#include "elgamal.h"
#include "dh.h"
#include "hash.h"
#include "blowfish.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include "test_utils.h"


#define STB_IMAGE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "third-party/stb-nothing/stb_image_write.h"


#define STB_VORBIS_IMPLEMENTATION
#include "third-party/stb-nothing/stb_vorbis.c"


#include <stdio.h>
#include <stdarg.h>

/* Print to stdout in the given color, then reset. */
static void cprintf(const char *color, const char *fmt, ...) {
    va_list ap;
    fputs(color, stdout);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    fputs(RESET, stdout);
}

/* Print to any stream (e.g. stderr) in the given color, then reset. */
static void cfprintf(FILE *stream, const char *color, const char *fmt, ...) {
    va_list ap;
    fputs(color, stream);
    va_start(ap, fmt);
    vfprintf(stream, fmt, ap);
    va_end(ap);
    fputs(RESET, stream);
}


static char *directory_input_images =  "results-images/original" ; 
static char *directory_output_images = "results-images/encrypted" ; 


#define CIPHER_SETKEY(name , key_cipher , _key , _key_len) name##_set_key(key_cipher , _key , _key_len)
#define CIPHER_ENCRYPT(name , original_text ,encrypted_text  ,length , key_cipher) name##_encrypt(original_text ,encrypted_text  ,length , key_cipher );


// hella ugly i know yea , thank you .
int dummy(char *algo , uchar_t *key_str , size_t key_len , uchar_t * original_text ,uchar_t * encrypted_text  ,size_t length) {

    if (strcmp(algo , "aes"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AesKey)) ;
        // cprintf( YELLOW , "INFO: setting key... \n");
        aes_set_key(key_cipher , key_str ,  key_len) ; 
        
        aes_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "des"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
        // cprintf( YELLOW , "INFO: setting key... \n");
        des_set_key(key_cipher , key_str ,  key_len) ; 
        des_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "tea"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
        // cprintf( YELLOW , "INFO: setting key... \n");
        tea_set_key(key_cipher , key_str ,  key_len) ; 
        tea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "xtea"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        
        // cprintf( YELLOW , "INFO: setting key... \n");
        xtea_set_key(key_cipher , key_str ,  key_len) ; 
        xtea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "redpike"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        
        // cprintf( YELLOW , "INFO: setting key... \n");
        redpike_set_key(key_cipher , key_str ,  key_len) ; 
        redpike_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "rc4"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        
        // cprintf( YELLOW , "INFO: setting key... \n");
        rc4_set_key(key_cipher , key_str ,  key_len) ; 
        rc4_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "blowfish"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        
        
        // cprintf( YELLOW , "INFO: setting key... \n");
        blowfish_set_key(key_cipher , key_str ,  key_len) ; 
        blowfish_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
    }
    else if (strcmp(algo , "affine"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
        // cprintf( YELLOW , "INFO: setting key... \n");
        affine_set_key(key_cipher , key_str ,  key_len) ; 
        affine_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 
        
        
    }
    
    else if (strcmp(algo , "hill"))
    {
        void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
        // cprintf( YELLOW , "INFO: setting key... \n");
        hill_set_key(key_cipher , key_str ,  key_len) ; 
        hill_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
        return 0 ; 

        
    }

    return 1 ; 
}

#define AES_BLOCK_SIZE 16
#define BLOWFISH_BLOCK_SIZE 8
#define DES_BLOCK_SIZE 8
#define REDPIKE_BLOCK_SIZE 8
#define TEA_BLOCK_SIZE 8
#define XTEA_BLOCK_SIZE 8


int handle_encryption(char *algo , uchar_t *key_str , size_t key_len , uchar_t * original_text ,size_t input_length , uchar_t ** encrypted_text  ,size_t *output_length) {

    if (!strcmp(algo , "aes"))
    {
        printf("aessss no one loves you \n") ; 
        void *key_cipher = (void*) calloc(1 , sizeof(AesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
            return 1;
        }
        // AesKey* aes_key = (AesKey*) key_cipher ;
        size_t reminder = input_length % AES_BLOCK_SIZE ; 
        uchar_t *temp_buf = original_text ; 
        size_t new_in_len = input_length  ; 

        *output_length = input_length ; 


        if (reminder != 0) {
            printf("hi lol \n") ; 
            printf("old length : %ld \n" , input_length) ; 
            new_in_len = aes_get_output_len(input_length) ;

            printf("new length : %ld \n" , new_in_len) ; 
            
            temp_buf = malloc(sizeof(uchar_t)*new_in_len) ;

            if (temp_buf == NULL) {
                fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_in_len ) ; 
                
                return 2 ; 
            } 


            *output_length = new_in_len ; 

            memcpy(temp_buf ,original_text , sizeof(uchar_t) * input_length ) ;
            memset(temp_buf + input_length , 0x00 , sizeof(uchar_t) * (new_in_len - input_length) ) ;  
        }

        *encrypted_text = malloc(*output_length * sizeof(uchar_t))  ;
        if (*encrypted_text == NULL) {
            fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n",*output_length *  sizeof(uchar_t) ) ; 
            if (reminder != 0) {
                free(temp_buf) ; 
            }    
            return  3;
        }
        printf("*out_len : %ld \n" , *output_length) ; 
        // cprintf( YELLOW , "INFO: setting key... \n");
        aes_set_key(key_cipher , key_str ,  key_len) ; 
        
        aes_encrypt(temp_buf ,*encrypted_text , new_in_len , key_cipher ) ;
        
        // memcpy(*encrypted_text ,encrypted_text , sizeof(uchar_t) * input_length ) ;

        if (reminder != 0) {
            free(temp_buf) ;;;;;;;
        }

        free(key_cipher) ; 
        
        return 0 ; 
        
    }
    // else if (!strcmp(algo , "des"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     des_set_key(key_cipher , key_str ,  key_len) ; 
    //     des_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "tea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     tea_set_key(key_cipher , key_str ,  key_len) ; 
    //     tea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "xtea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     xtea_set_key(key_cipher , key_str ,  key_len) ; 
    //     xtea_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "redpike"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     redpike_set_key(key_cipher , key_str ,  key_len) ; 
    //     redpike_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "rc4"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     rc4_set_key(key_cipher , key_str ,  key_len) ; 
    //     rc4_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "blowfish"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     blowfish_set_key(key_cipher , key_str ,  key_len) ; 
    //     blowfish_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "affine"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     affine_set_key(key_cipher , key_str ,  key_len) ; 
    //     affine_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
        
    // }
    
    // else if (!strcmp(algo , "hill"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     hill_set_key(key_cipher , key_str ,  key_len) ; 
    //     hill_encrypt(original_text ,encrypted_text , length , key_cipher ) ;
    //     return 0 ; 

        
    // }

    return 1 ; 
}



// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int encrypt_image_file(char *name  , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len) {


    printf("INFO: Testing %s... \n" , name  ) ; 
    printf("INFO: TYPE Image \n"   ) ; 

    
    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
        fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 
        return 1 ;
    }
    
    size_t length = width * height * channels ; 
    
    // this is a lil bit risky yk ...


    uchar_t *encrypted_text = NULL;


    size_t out_length = 0 ; 
    
    int enc_result = handle_encryption(name ,_key , _key_len , original_text  , length , &encrypted_text , &out_length   ) ;
    if (enc_result != 0) {
        fprintf(stderr, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;
        
        free(encrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }
    printf(  "INFO: encrypted with success\n" );
    
    printf("atf\n") ; 
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, encrypted_text, width * channels);
    printf("atf\n") ; 
    if (__res_funv_ == 0) {
        fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        free(encrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }
    
    printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    
    free(encrypted_text);
    stbi_image_free(original_text);
    
    
    return  0;


}


int handle_decryption(char *algo , uchar_t *key_str , size_t key_len , uchar_t * original_text ,uchar_t * encrypted_text  ,size_t length) {

    if (!strcmp(algo , "aes"))
    {
        printf("aessss no one loves you \n") ; 
        void *key_cipher = (void*) calloc(1 , sizeof(AesKey)) ;
        if (key_cipher == NULL) {
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
            return 1;
        }
        // AesKey* aes_key = (AesKey*) key_cipher ;
        size_t reminder = length % AES_BLOCK_SIZE ; 
        uchar_t *temp_buf = original_text ; 
        uchar_t *temp_out = encrypted_text ; 
        size_t new_len = length  ; 
        
        if (reminder != 0) {
            new_len = length + (AES_BLOCK_SIZE - reminder) ;
            
            temp_buf = malloc(sizeof(uchar_t)*new_len) ;

            if (temp_buf == NULL) {
                fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_len ) ; 

                return 2 ; 
            } 

            temp_out = malloc(sizeof(uchar_t)*new_len) ;

            if (temp_out == NULL) {
                fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_len ) ; 

                return 3 ; 
            } 

            memcpy(temp_buf ,original_text , sizeof(uchar_t) * length ) ;
            memset(temp_buf + length , 0x0 , sizeof(uchar_t) * (new_len - length) ) ;  
        }

        // cprintf( YELLOW , "INFO: setting key... \n");
        aes_set_key(key_cipher , key_str ,  key_len) ; 
        
        aes_decrypt(temp_buf ,temp_out , new_len , key_cipher ) ;
        
        memcpy(encrypted_text ,temp_out , sizeof(uchar_t) * length ) ;

        if (reminder != 0) {
            free(temp_out) ;;;;;;;
            free(temp_buf) ;;;;;;;
        }
        return 0 ; 
        
    }
    // else if (!strcmp(algo , "des"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(DesKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     des_set_key(key_cipher , key_str ,  key_len) ; 
    //     des_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "tea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(TeaKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     tea_set_key(key_cipher , key_str ,  key_len) ; 
    //     tea_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "xtea"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(XTeaKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     xtea_set_key(key_cipher , key_str ,  key_len) ; 
    //     xtea_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "redpike"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(RedpikeKey)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     redpike_set_key(key_cipher , key_str ,  key_len) ; 
    //     redpike_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "rc4"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(Rc4Key)) ;
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     rc4_set_key(key_cipher , key_str ,  key_len) ; 
    //     rc4_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "blowfish"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(BlowfishKey)) ;
        
        
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     blowfish_set_key(key_cipher , key_str ,  key_len) ; 
    //     blowfish_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
    // }
    // else if (!strcmp(algo , "affine"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(AffineKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     affine_set_key(key_cipher , key_str ,  key_len) ; 
    //     affine_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 
        
        
    // }
    
    // else if (!strcmp(algo , "hill"))
    // {
    //     void *key_cipher = (void*) calloc(1 , sizeof(HillKey)) ;
    //     // cprintf( YELLOW , "INFO: setting key... \n");
    //     hill_set_key(key_cipher , key_str ,  key_len) ; 
    //     hill_decrypt(original_text ,decrypted_text , length , key_cipher ) ;
    //     return 0 ; 

        
    // }

    return 1 ; 
}



// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int decrypt_image_file(char *name  , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len) {


    printf("INFO: Testing %s... \n" , name  ) ; 
    printf("INFO: TYPE Image \n"   ) ; 

    
    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
        fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 
        return 1 ;
    }
    
    int length = width * height * channels ; 
    
    
    uchar_t *decrypted_text = malloc(sizeof(uchar_t) * length );
    if (decrypted_text == NULL) {
        fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n" , sizeof(uchar_t) * length) ; 
        stbi_image_free(original_text);    
        return 2 ;
    }

    
    int enc_result = handle_decryption(name ,_key , _key_len , original_text , decrypted_text , length ) ;
    if (enc_result != 0) {
        fprintf(stderr, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;
        
        free(decrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }
    printf(  "INFO: encrypted with success\n" );
    
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, decrypted_text, width * channels);
    if (__res_funv_ == 0) {
        fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        free(decrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }
    
    printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    
    free(decrypted_text);
    stbi_image_free(original_text);
    
    
    return  0;


}






int main() {

    uchar_t input[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5, 0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28, 
                        0xf4, 0x43, 0xe3, 0xca, 0x4d, 0x62, 0xb5, 0x9a, 0xca, 0x84, 0xe9, 0x90, 0xca, 0xca, 0xf5, 0xc5, 
                        0x2b, 0x09, 0x30, 0xda, 0xa2, 0x3d, 0xe9, 0x4c, 0xe8, 0x70, 0x17, 0xba, 0x2d, 0x84, 0x98, 0x8d, 
                        0xdf, 0xc9, 0xc5, 0x8d, 0xb6, 0x7a, 0xad, 0xa6, 0x13, 0xc2, 0xdd, 0x08, 0x45, 0x79, 0x41, 0xa6  } ;
    uchar_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }  ;
    size_t key_len = 32 ; 
    uchar_t output[64]  ;
    uchar_t output2[64]  ;


    uchar_t buffer[] = {0x1 , 0x2 , 0x3 , 0x4 , 0x5 , 0x6 , 0x7 , 0x8 , 
                    0x9 , 0x10 , 0x11 , 0x12 } ; 

    int length = 12 ;
    uchar_t temp_buf[32] = {0} ; 
    uchar_t temp_out[32] = {0} ; 
    uchar_t temp_dec[32] = {0} ; 

    printf("before : \n") ; 
    PRINT_ARRAY_NAI(temp_buf, 32, "%x") ; 
    
    // padding 
    
    memcpy(temp_buf ,buffer , sizeof(uchar_t) * length ) ;
    memset(temp_buf + length , 0xff , sizeof(uchar_t) * (16 - length ) ) ;      

    printf("after : \n") ; 
    PRINT_ARRAY_NAI(temp_buf, 32, "%x") ; 
    
    void *key_aes = malloc(sizeof(AesKey)) ; 
    if (key_aes==NULL) {
        printf("hell nas") ; 
        abort() ;
    }

    aes_set_key(key_aes, key, key_len) ; 

    aes_encrypt(temp_buf, temp_out, 16, key_aes) ; 
    printf("output of encryption : \n") ; 
    PRINT_ARRAY_NAI(temp_out, 32, "%x") ; 
    memset(temp_out + 12 , 0xff, 4) ; 
    aes_decrypt(temp_out, temp_dec, 16, key_aes) ; 

    printf("output of decryption : \n") ; 
    PRINT_ARRAY_NAI(temp_dec, 32, "%x") ; 
    
    // return 0;


    printf("hi lol \n") ; 
    // fuck("aes" , "coast.jpg"  , key ,key_len  ) ; 
    encrypt_image_file("aes", "results-images/original/coast.jpg", "results-images/encrypted/coast-haha3.png", key,  key_len) ; 
    decrypt_image_file("aes", "results-images/encrypted/coast-haha3.png", "results-images/decrypted/coast-haha-dec3.png", key,  key_len) ; 






    return 0 ; 
}