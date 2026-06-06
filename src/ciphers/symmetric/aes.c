#ifndef AES_C
#define AES_C


#include "aes.h"
// #include "aes_lib.h"   // the tiny-AES library
#include <stdlib.h>
#include <string.h>
#include <stdio.h>     



#include <stdio.h>
#include "../../common/keyexpan.h"

#include "../../common/galois_field_op.h"
#include "../../common/rijnbox.h"







void fill_state(uchar_t *input  , int index, uchar_t state[4][4]) {
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = input[index] ; 
            index++ ; 
        }
        
    }
    
}


void add_round_key(uchar_t *key , int k , uchar_t state[4][4]) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] ^= key[k] ;
            k++ ;  
        }
        
    }
    
}

/*
    the function that is used in AES rounds !!!!
*/
void sub_bytes(uchar_t state[4][4] ) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = subByte(state[j][i]) ; 
        }
        
    }
    
}




// Function to reverse a portion of the array
void reverse(uchar_t* arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// Function to rotate an array by d elements to the left
void rotateArr(uchar_t* arr, int n, int d) {
    
    // Handle the case where d > size of array
    d %= n;

    // Reverse the first d elements
    reverse(arr, 0, d - 1);

    // Reverse the remaining n-d elements
    reverse(arr, d, n - 1);

    // Reverse the entire array
    reverse(arr, 0, n - 1);
}



void shift_rows(uchar_t state[4][4]) {
    int rotation_num = 0 ; 
    for (int i = 0; i < 4; i++)
    {
        rotateArr(state[i] ,4 , rotation_num ) ;         
        rotation_num++ ; 
    }
    
}



void mix_culumns(uchar_t state[4][4]) {
    uchar_t temp0 , temp1 , temp2 , temp3 ; 
    for (int i = 0; i < 4; i++)
    {
        temp0 = state[0][i] ; 
        temp1 = state[1][i] ; 
        temp2 = state[2][i] ; 
        temp3 = state[3][i] ; 

        state[0][i] = ( mod_GF(mul_GF_16bit(0x02 , temp0 )) ^ mod_GF(mul_GF_16bit(0x03 , temp1 )) ^  temp2 ^ temp3) ;   
        
        state[1][i] = temp0 ^ mod_GF(mul_GF_16bit(0x02 , temp1 )) ^ mod_GF(mul_GF_16bit(0x03 , temp2 )) ^ temp3 ;   
        
        state[2][i] = temp0 ^ temp1 ^  mod_GF(mul_GF_16bit(0x02 , temp2 )) ^ mod_GF(mul_GF_16bit(0x03 , temp3 )) ;   
        
        state[3][i] = mod_GF(mul_GF_16bit(0x03 , temp0 )) ^ temp1 ^  temp2 ^ mod_GF(mul_GF_16bit(0x02 , temp3 )) ;   
    }
    
}


void fill_state_inv(uchar_t *key  , int k, uchar_t state[4][4]) {
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // printf("")
            key[k] = state[j][i] ; 
            k++ ; 
        }
        
    }
    
}



void aes_cipher_block(uchar_t *input   , uchar_t *output , uchar_t *key , int Nr) {
    // printf("\t\t *************************** AES ***************************\n") ; 
    
    uchar_t state[4][4] ; 
    fill_state(input , 0 , state) ;


    add_round_key(key , 0 , state) ; 


    for (int i = 1; i <= Nr -1 ; i++)
    {

        sub_bytes(state) ; 
        
        shift_rows(state) ; 

        mix_culumns(state) ; 

        add_round_key(key , 4 * i * 4  , state) ; 

    }

    sub_bytes(state) ; 

    shift_rows(state) ; 

    add_round_key(key , 4 * Nr * 4  , state) ; 
    
    fill_state_inv(output , 0 , state) ; 

}


/* 
    for now it only works with perfect buffer of size k * 16 (k > 0)
*/
void aes_cipher(uchar_t *input   , uchar_t *output, uchar_t *key  , int length, int Nr) {
    

    for (int i = 0; i < length / 16; i++)
    {
        int margin = i * 16 ; 
        aes_cipher_block(input  + margin , output + margin  ,key , Nr ) ;

    }
    
}


void setup_parameteres_aes(AES_TYPE type , int *Nr , int *Nk) {
    if (type == AES128 ) {
        *Nk = 4 ; 
        *Nr = 10 ; 
    }
    if (type == AES192 ) {
        *Nk = 6 ; 
        *Nr = 12 ; 
    }
    if (type == AES256 ) {
        *Nk = 8 ; 
        *Nr = 14 ; 
    }
}











void aes_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{


}



void aes_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
}


#include <assert.h>

void aes_set_key(void* key_struct, const char* key_str)
{
}



void aes_free_key(void* key_struct)
{
}







#endif