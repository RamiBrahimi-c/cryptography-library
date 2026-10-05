/*
    basically what this file serves as is just to show visual encrypt and decrypt on images using our symmetric algorithms 

*/


#include "affine.h"
#include "hill.h"
#include "substitution.h"
#include "rc4.h"
#include "des.h"
#include "3des.h"
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


#define CIPHER_SETKEY(name , key_cipher , _key , _key_len) name##_set_key(key_cipher , _key , _key_len)
#define CIPHER_ENCRYPT(name , original_text ,encrypted_text  ,length , key_cipher) name##_encrypt(original_text ,encrypted_text  ,length , key_cipher );



#define AES_BLOCK_SIZE 16
#define BLOWFISH_BLOCK_SIZE 8
#define DES_BLOCK_SIZE 8
#define TDES_BLOCK_SIZE 8
#define REDPIKE_BLOCK_SIZE 8
#define TEA_BLOCK_SIZE 8
#define XTEA_BLOCK_SIZE 8

static char *directory_input_images =  "results-images/original" ; 
static char *directory_output_images = "results-images/encrypted" ; 



#define PRETTY_INFO_FLAG 1
#define INFO_FLAG 0
#define DEBUG_FLAG 0


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


static size_t get_output_len_pad(char *cipher ,size_t input_len) {
    if (strcmp(cipher, "aes") == 0) return (size_t) (input_len + (AES_BLOCK_SIZE - (input_len % AES_BLOCK_SIZE)));
    if (strcmp(cipher, "des") == 0) return (size_t) (input_len + (DES_BLOCK_SIZE - (input_len % DES_BLOCK_SIZE)));
    if (strcmp(cipher, "3des") == 0) return (size_t) (input_len + (TDES_BLOCK_SIZE - (input_len % TDES_BLOCK_SIZE)));
    if (strcmp(cipher, "blowfish") == 0) return (size_t) (input_len + (BLOWFISH_BLOCK_SIZE - (input_len % BLOWFISH_BLOCK_SIZE)));
    if (strcmp(cipher, "redpike") == 0) return (size_t) (input_len + (REDPIKE_BLOCK_SIZE - (input_len % REDPIKE_BLOCK_SIZE)));
    if (strcmp(cipher, "tea") == 0) return (size_t) (input_len + (TEA_BLOCK_SIZE - (input_len % TEA_BLOCK_SIZE)));
    if (strcmp(cipher, "xtea") == 0) return (size_t) (input_len + (XTEA_BLOCK_SIZE - (input_len % XTEA_BLOCK_SIZE)));
    if (strcmp(cipher, "rc4") == 0) return (size_t) (input_len );
 
    return 0 ; 
}

static int get_block_size(const char* cipher_name) {
    if (!cipher_name) return 0;
    if (strcmp(cipher_name, "aes") == 0)      return 16;
    if (strcmp(cipher_name, "des") == 0)      return 8;
    if (strcmp(cipher_name, "3des") == 0)      return 8;
    if (strcmp(cipher_name, "blowfish") == 0) return 8;
    if (strcmp(cipher_name, "tea") == 0)      return 8;
    if (strcmp(cipher_name, "xtea") == 0)     return 8;
    if (strcmp(cipher_name, "redpike") == 0)     return 8;
    if (strcmp(cipher_name, "rc4") == 0)      return 1;
    // classical ciphers operate byte-wise → 1
    if (strcmp(cipher_name, "caesar") == 0)   return 1;
    if (strcmp(cipher_name, "vigenere") == 0) return 1;
    if (strcmp(cipher_name, "affine") == 0)   return 1;
    // unknown
    return 0;
}



static int cipher_encrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key);

static int cipher_decrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key) ;



static void* get_cipher_keysruct(char *cipher ) {
    if (strcmp(cipher, "aes") == 0) return (void*) calloc(1 , sizeof(AesKey));
    if (strcmp(cipher, "des") == 0) return (void*) calloc(1 , sizeof(DesKey));
    if (strcmp(cipher, "3des") == 0) return (void*) calloc(1 , sizeof(TDesKey));
    if (strcmp(cipher, "blowfish") == 0) return (size_t) (void*) calloc(1 , sizeof(BlowfishKey));
    if (strcmp(cipher, "redpike") == 0) return (size_t) (void*) calloc(1 , sizeof(RedpikeKey));
    if (strcmp(cipher, "tea") == 0) return (size_t) (void*) calloc(1 , sizeof(TeaKey));
    if (strcmp(cipher, "xtea") == 0) return (size_t) (void*) calloc(1 , sizeof(XTeaKey));
    if (strcmp(cipher, "rc4") == 0) return (size_t) (void*) calloc(1 , sizeof(Rc4Key));
 
    return NULL ; 
}

static int set_cipher_key(char *cipher , void *key_struct, const uchar_t *key_str, size_t key_len) {
    if (strcmp(cipher, "aes") == 0)        return aes_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "des") == 0)        return des_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "3des") == 0)       return tdes_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "blowfish") == 0)   return blowfish_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "redpike") == 0)    return redpike_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "tea") == 0)        return tea_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "xtea") == 0)       return xtea_set_key(key_struct, key_str,  key_len);
    if (strcmp(cipher, "rc4") == 0)        return rc4_set_key(key_struct, key_str,  key_len);
 
    return NULL ; 
}

int handle_encryption(char *algo ,char *mode, uchar_t *key_str , size_t key_len , uchar_t * original_text ,size_t input_length , uchar_t ** encrypted_text  ,size_t *output_length , uchar_t *iv) {

    void *key_cipher = get_cipher_keysruct(algo) ;
    if (key_cipher == NULL) {

        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 

        #endif

        return 1;
    }
    size_t reminder = input_length % get_block_size(algo) ; 
    uchar_t *temp_buf = original_text ; 
    size_t new_in_len = input_length  ; 

        *output_length = input_length ; 

    #if INFO_FLAG
        printf("reminder : %ld\n" , reminder) ; 
    #endif

    if (reminder != 0) {
        // printf("old length : %ld \n" , input_length) ; 
        new_in_len = get_output_len_pad(algo , input_length) ;
        if (new_in_len == 0) {

            #if PRETTY_INFO_FLAG
                cfprintf(stderr , RED, "ERROR: errot happened at get_output_len_pad we got 0 back \n")  ;
            #elif INFO_FLAG 
                fprintf(stderr, "ERROR: errot happened at get_output_len_pad we got 0 back \n")  ;

            #endif            
            
            return 3;
        }
        
        temp_buf = malloc(sizeof(uchar_t)*new_in_len) ;
        if (temp_buf == NULL) {
            #if PRETTY_INFO_FLAG
                cfprintf(stderr , RED, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_in_len ) ; 
            #elif INFO_FLAG 
                fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_in_len ) ; 

            #endif            


                
            return 2 ; 
        } 


        *output_length = new_in_len ; 

        memcpy(temp_buf ,original_text , sizeof(uchar_t) * input_length ) ;
        memset(temp_buf + input_length , 0x00 , sizeof(uchar_t) * (new_in_len - input_length) ) ;  
    }

    #if INFO_FLAG
    printf("*output_length = %ld\n" , *output_length) ; 
    #endif

    *encrypted_text = malloc(*output_length * sizeof(uchar_t))  ;
    if (*encrypted_text == NULL) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : malloc failed to allocate %ld bytes \n",*output_length *  sizeof(uchar_t) ) ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n",*output_length *  sizeof(uchar_t) ) ; 

        #endif            

        if (reminder != 0) {
            free(temp_buf) ; 
        }    
        return  4;
    }

    int set_res = set_cipher_key(algo, key_cipher, key_str, key_len) ;  
    if (set_res != 0) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : error happened at set key phase\n") ; 
        #elif INFO_FLAG 
            fprintf(stderr , "ERROR : error happened at set key phase\n") ; 
        #endif            

        return 5 ; 
    }
    int enc_res = cipher_encrypt_mode(algo, mode,temp_buf , *encrypted_text,  iv, new_in_len, key_cipher)  ;
    if (enc_res != 0) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : error happened at set cipher_encrypt_mode\n") ; 
        #elif INFO_FLAG 
            fprintf(stderr , "ERROR : error happened at set cipher_encrypt_mode\n") ; 
        #endif            


        return 6 ; 
    }        

    if (reminder != 0) {
        free(temp_buf) ;;;;;;;
    }

    free(key_cipher) ; 
        
    return 0 ; 
        
}



// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int encrypt_image_file(char *name , char *mode , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len , uchar_t *iv) {

    #if PRETTY_INFO_FLAG
        cprintf(BLUE , "INFO: Testing %s... \n" , name  ) ; 
        cprintf(GREEN , "INFO: TYPE Image \n"   ) ; 
    #elif INFO_FLAG 
        printf( "INFO: Testing %s... \n" , name  ) ; 
        printf( "INFO: TYPE Image \n"   ) ; 

    #endif

    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
    
        #if PRETTY_INFO_FLAG
            cfprintf( stderr , RED, "ERROR : couldnt extract pixels from original picture \n") ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 

        #endif
    
        return 1 ;
    }
    
    size_t length = width * height * channels ; 
    
    // this is a lil bit risky yk ...


    uchar_t *encrypted_text = NULL;


    size_t out_length = 0 ; 
    
    // just some detail to prevent issues
    if ( iv == NULL || strcmp(mode , "ecb")==0 ) {
        // printf("wth\n")  ;
        // strcpy(mode , "ecb") ; 
    }

    int enc_result = handle_encryption(name ,mode, _key , _key_len ,  original_text , length , &encrypted_text  ,&out_length , iv) ;
    
    if (enc_result != 0) {
        

        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : handle for encryption failed (code %d)\n" , enc_result ) ;

        #endif        
        

        free(encrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }

    #if PRETTY_INFO_FLAG
        cprintf(GREEN ,   "INFO: encrypted with success\n" );
    #elif INFO_FLAG 
        printf(  "INFO: encrypted with success\n" );

    #endif
    
    
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, encrypted_text, width * channels);
    if (__res_funv_ == 0) {
    

        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : stbi write png failed \n") ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        #endif    

        free(encrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }
    

    #if PRETTY_INFO_FLAG
        cprintf(YELLOW ,  "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );
    #elif INFO_FLAG 
        printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    #endif

    free(encrypted_text);
    stbi_image_free(original_text);
   
    
    return  0;


}


int handle_decryption(char *algo ,char *mode, uchar_t *key_str , size_t key_len , uchar_t * original_text ,size_t input_length , uchar_t ** encrypted_text  ,size_t *output_length , uchar_t *iv) {

    void *key_cipher = get_cipher_keysruct(algo) ;
    if (key_cipher == NULL) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : calloc failed to allocate %ld bytes \n", sizeof(AesKey) ) ; 
        #endif            

        return 1;
    }
    size_t reminder = input_length % get_block_size(algo) ; 
    uchar_t *temp_buf = original_text ; 
    size_t new_in_len = input_length  ; 

    *output_length = input_length ; 

    #if INFO_FLAG
        printf("reminder : %ld \n" , reminder) ; 
    #endif
    if (reminder != 0) {
        // printf("old length : %ld \n" , input_length) ; 
        new_in_len = get_output_len_pad(algo , input_length) ;
        if (new_in_len == 0) {
            #if PRETTY_INFO_FLAG
                cfprintf(stderr , RED, "ERROR: errot happened at get_output_len_pad we got 0 back \n")  ;
            #elif INFO_FLAG 
                fprintf(stderr, "ERROR: errot happened at get_output_len_pad we got 0 back \n")  ;
            #endif            
            
            return 3;
        }
        #if INFO_FLAG
            printf("new length : %ld \n" , new_in_len) ; 
        #endif
        
        temp_buf = malloc(sizeof(uchar_t)*new_in_len) ;
        if (temp_buf == NULL) {
            #if PRETTY_INFO_FLAG
                cfprintf(stderr , RED, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_in_len ) ; 
            #elif INFO_FLAG 
                fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n", sizeof(uchar_t)*new_in_len ) ; 
            #endif            
            
                
            return 2 ; 
        } 


        *output_length = new_in_len ; 

        memcpy(temp_buf ,original_text , sizeof(uchar_t) * input_length ) ;
        memset(temp_buf + input_length , 0x00 , sizeof(uchar_t) * (new_in_len - input_length) ) ;  
    }

    *encrypted_text = malloc(*output_length * sizeof(uchar_t))  ;
    if (*encrypted_text == NULL) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : malloc failed to allocate %ld bytes \n",*output_length *  sizeof(uchar_t) ) ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : malloc failed to allocate %ld bytes \n",*output_length *  sizeof(uchar_t) ) ; 
        #endif            

        if (reminder != 0) {
            free(temp_buf) ; 
        }    
        return  3;
    }
    // printf("*out_len : %ld \n" , *output_length) ; 
    // cprintf( YELLOW , "INFO: setting key... \n");
    int set_res = set_cipher_key(algo, key_cipher, key_str, key_len) ;  
    if (set_res != 0) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED , "ERROR : error happened at set key phase\n") ; 
        #elif INFO_FLAG 
            fprintf(stderr , "ERROR : error happened at set key phase\n") ; 
        #endif            

        return 4 ; 
    }
    int dec_res = cipher_decrypt_mode(algo, mode,temp_buf , *encrypted_text,  iv, new_in_len, key_cipher)  ;
    if (dec_res != 0) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : error happened at set cipher_encrypt_mode\n") ; 
        #elif INFO_FLAG 
            fprintf(stderr , "ERROR : error happened at set cipher_encrypt_mode\n") ; 
        #endif            

        return 5 ; 
    }        

    if (reminder != 0) {
        free(temp_buf) ;;;;;;;
    }

    free(key_cipher) ; 
        
    return 0 ; 
        

}



// return 0 in SUCCESS otherwise its positive integer with error printed in stderr 
int decrypt_image_file(char *name , char *mode , uchar_t *input_path,uchar_t* output_path, uchar_t * _key ,size_t _key_len , uchar_t *iv) {


    #if PRETTY_INFO_FLAG
        cprintf(BLUE , "INFO: Testing %s... \n" , name  ) ; 
        cprintf(GREEN , "INFO: TYPE Image \n"   ) ; 
    #elif INFO_FLAG 
        printf( "INFO: Testing %s... \n" , name  ) ; 
        printf( "INFO: TYPE Image \n"   ) ; 

    #endif

    
    int width, height, channels;
    char *algo_name = name ; 
    
    
    uchar_t *original_text = stbi_load(input_path, &width, &height, &channels, 0);
    if (original_text == NULL) {
        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : couldnt extract pixels from original picture \n") ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : couldnt extract pixels from original picture \n") ; 
        #endif            

        return 1 ;
    }
    
    size_t length = width * height * channels ; 
    
    
    uchar_t *decrypted_text = NULL ; 
    size_t out_length = 0 ; 

    
    int dec_result = handle_decryption(name , mode , _key , _key_len , original_text , length , &decrypted_text , &out_length , iv ) ;
    if (dec_result != 0) {

        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : handle for decryption failed (code %d)\n" , dec_result ) ;
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : handle for decryption failed (code %d)\n" , dec_result ) ;

        #endif

        
        free(decrypted_text) ; 
        stbi_image_free(original_text);
        return 3; 
    }

    #if PRETTY_INFO_FLAG
        cprintf(GREEN ,   "INFO: decrypted with success\n" );
    #elif INFO_FLAG 
        printf(  "INFO: decrypted with success\n" );

    #endif


    
    int __res_funv_ = stbi_write_png(output_path, width, height, channels, decrypted_text, width * channels);
    if (__res_funv_ == 0) {

        #if PRETTY_INFO_FLAG
            cfprintf(stderr , RED, "ERROR : stbi write png failed \n") ; 
        #elif INFO_FLAG 
            fprintf(stderr, "ERROR : stbi write png failed \n") ; 

        #endif


        free(decrypted_text) ; 
        stbi_image_free(original_text);

        return 4;
    }


    #if PRETTY_INFO_FLAG
        cprintf(YELLOW ,  "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );
    #elif INFO_FLAG 
        printf( "INFO: saved to %s (code %d ) \n" , output_path , __res_funv_  );

    #endif


    
    free(decrypted_text);
    stbi_image_free(original_text);
    
    
    return  0;


}






int main() {

    uchar_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 }  ;
    size_t key_len = 32 ; 

    uchar_t iv[] = {
        0x0 , 0x1 , 0x2 , 0x3 , 0x4 , 0x5 , 0x6 , 0x7 , 0x8 , 0x9 , 0xa , 0xb , 0xc , 0xd , 0xe , 0xf
    };
    size_t iv_len = 16  ; 


    printf("hi lol \n") ; 
    encrypt_image_file("gghgghg" , "ofb", "results-images/original/Tux.png", "results-images/encrypted/Tux-aes-ofb.png", key,  key_len , iv) ; 
    decrypt_image_file("aes" , "ofb" , "results-images/encrypted/Tux-aes-ofb.png", "results-images/decrypted/Tux-aes-ofb.png", key,  key_len , iv) ; 



    return 0 ; 
}




static int cipher_encrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key) {
    if (strcmp(cipher, "aes") == 0) {
        if (strcmp(mode, "ecb") == 0) return aes_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return aes_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return aes_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return aes_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return aes_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "des") == 0) {
        if (strcmp(mode, "ecb") == 0) return des_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return des_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return des_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return des_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return des_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "3des") == 0) {
        if (strcmp(mode, "ecb") == 0) return tdes_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tdes_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tdes_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tdes_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tdes_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "blowfish") == 0) {
        if (strcmp(mode, "ecb") == 0) return blowfish_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return blowfish_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return blowfish_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return blowfish_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return blowfish_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "redpike") == 0) {
        if (strcmp(mode, "ecb") == 0) return redpike_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return redpike_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return redpike_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return redpike_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return redpike_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "tea") == 0) {
        if (strcmp(mode, "ecb") == 0) return tea_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tea_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tea_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tea_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tea_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "xtea") == 0) {
        if (strcmp(mode, "ecb") == 0) return xtea_encrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return xtea_encrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return xtea_encrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return xtea_encrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return xtea_encrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "rc4") == 0) {
        return rc4_encrypt(in, out, length, key);
    }

    return -1;
}

static int cipher_decrypt_mode(const char* cipher, const char* mode,
                        const uchar_t* in, uchar_t* out, const uchar_t* iv,
                        int length, const void* key) {
    if (strcmp(cipher, "aes") == 0) {
        if (strcmp(mode, "ecb") == 0) return aes_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return aes_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return aes_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return aes_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return aes_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "des") == 0) {
        if (strcmp(mode, "ecb") == 0) return des_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return des_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return des_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return des_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return des_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "3des") == 0) {
        if (strcmp(mode, "ecb") == 0) return tdes_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tdes_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tdes_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tdes_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tdes_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "blowfish") == 0) {
        if (strcmp(mode, "ecb") == 0) return blowfish_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return blowfish_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return blowfish_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return blowfish_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return blowfish_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "redpike") == 0) {
        if (strcmp(mode, "ecb") == 0) return redpike_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return redpike_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return redpike_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return redpike_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return redpike_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "tea") == 0) {
        if (strcmp(mode, "ecb") == 0) return tea_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return tea_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return tea_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return tea_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return tea_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "xtea") == 0) {
        if (strcmp(mode, "ecb") == 0) return xtea_decrypt(in, out, length, key);
        if (strcmp(mode, "cbc") == 0) return xtea_decrypt_cbc(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "cfb") == 0) return xtea_decrypt_cfb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ofb") == 0) return xtea_decrypt_ofb(in, out, (uchar_t*)iv, length, key);
        if (strcmp(mode, "ctr") == 0) return xtea_decrypt_ctr(in, out, (uchar_t*)iv, length, key);
    }
    if (strcmp(cipher, "rc4") == 0) {
        return rc4_decrypt(in, out, length, key);
    }

    return -1;
}

