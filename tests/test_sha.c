#include "hash.h"
#include <math.h>

// maybe there is a better way to do this shit :
static void init_Hash_Values() {
    const int PRIMES_64[64] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
        283, 293, 307, 311
    };

    int b ;
    long double d ;  
    int j = 0 ;
    for (int i = 0; i < 8*4; i+=4)
    {
        d = sqrt(PRIMES_64[j]) * 4294967296 ;
        b = (uint32_t) d ; // undefined behavior ; set uint32_t to uint64_t
        printf(" b = %u \t %x  \n" , b , b) ; 
        j++ ;  

    }


}



static void init_K() {
    const int PRIMES_64[64] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 
        283, 293, 307, 311
    };

    int b ;
    long double d ;  
    int j = 0 ; 
    for (int i = 0; i < 64*4; i+=4)
    {
        d = cbrt(PRIMES_64[j]) * 4294967296 ;
        b = (uint32_t) d ;
        printf(" b = %u \t %x  \n" , b , b) ; 
        j++ ; 
    }
        
}

int main() {
    
    uchar_t K[1] ; 
    uchar_t H[1] ; 

    init_K() ; 
    init_Hash_Values() ; 
 
    return 0 ; 
 
    const unsigned char* s = (const unsigned char*)"abc";
    unsigned char out[32];
    sha256_hash(s, 3, out);
    for (int i = 0; i < 32; i++) printf("%02x", out[i]);
    printf("\n");
    return 0;
}