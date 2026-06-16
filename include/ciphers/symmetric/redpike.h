#ifndef REDPIKE_H
#define REDPIKE_H


#include "../../cipher_interface.h"
#include "../../common/constants.h"
#include <stdint.h>

#define REDPIKE_KEY_MAX_SIZE 8
#define CONST 0x9E3779B9




typedef struct {
    uchar_t key[REDPIKE_KEY_MAX_SIZE] ; 
    uint32_t constant ;
} RedpikeKey;





void redpike_encrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void redpike_decrypt(const uchar_t* input, uchar_t* output , int length , const void* key);
void redpike_set_key(void* key_struct, const char* key_str);
void redpike_free_key(void* key_struct);

Cipher* get_redpike_cipher(void);













#endif