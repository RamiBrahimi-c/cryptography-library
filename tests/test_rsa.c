// test_rsa_only.c
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "rsa.h"




int main() {
    printf("Start\n");
    fflush(stdout);
    
    // test_rsa_32bytes(512);
    // test_rsa_32bytes(1024);
    // test_rsa_32bytes(2048);
    

    void *key = malloc(sizeof(RsaKey)) ; 
    RsaKey *rsa_key ; 
    rsa_set_key(key ,"10:10:10:10" ) ; 
    printf("hi\n") ; 
    rsa_key = (RsaKey*) key ; 
    printf("hi\n") ; 
    printf("") ; 
    rsa_key->bits ; 
    printf("hi after\n") ; 
    mpz_t a ;

    mpz_init(&a) ; 
    mpz_set_str(&a , "123456789" , 10) ; 

    gmp_printf("a : %Zd\n" , a) ; 
    gmp_printf("bits : %d\n" , rsa_key->bits) ; 

    gmp_printf("d : %Zd\n" , rsa_key->d) ; 
    gmp_printf("e : %Zd\n" , rsa_key->e) ; 
    gmp_printf("n : %Zd\n" , rsa_key->n) ; 


    // hybrid_test() ; 


    printf("Done\n");
    return 0;
}