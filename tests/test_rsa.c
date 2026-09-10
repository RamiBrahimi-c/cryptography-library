#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "aes.h"

#include "bigra9m.h"





int main() {
    printf("Start\n");
    fflush(stdout);
    
    // hybrid_test() ; 


    // exit(EXIT_SUCCESS) ; 

    // test_rsa_32bytes(512);
    // test_rsa_32bytes(1024);
    // test_rsa_32bytes(2048);
    BigInt rami ; 

    bigra9m_init_str(&rami , "123") ; 
    bigra9m_print(rami) ; 
    RsaKey rsa_key2  ; 
    clock_t before , after ;

    
    bigra9m_inits(&rsa_key2.d ,&rsa_key2.e , &rsa_key2.n , NULL ) ;
    
    int bits = 256 ; 
    printf("========================= key 1 =================================\n") ; 
    before = clock() ; 
    rsa_generate_keypair(&rsa_key2 , bits , 13793) ; 
    after = clock() ; 
    
    rsa_export_key(&rsa_key2 ) ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    
    
    uchar_t input[10] = {0x01 , 0x23 , 0x45 , 0x67 , 0x89 , 0xab , 0xcd  } ; 
    size_t input_len = 4 ;
    uchar_t output[1024] , decryption_buf[1024] ; 
    uchar_t output2[1024] , decryption_buf2[1024] ; 
    
    printf("======================================================\n") ; 
    printf("======================================================\n") ; 
    printf("input : \n") ; 
    PRINT_ARRAY_NAI(input , input_len , "%x") ; 
    printf("======================================================\n") ; 
    printf("======================================================\n") ; 
    
    
    printf("======================================================\n") ; 
    printf("======================================================\n") ; 
    printf("enc with bigra9m : \n") ;
    
    before = clock() ;
    rsa_encrypt(input , output2 , input_len , &rsa_key2) ; 
    after = clock() ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    PRINT_ARRAY_NAI(output2 , 64 , "%x") ; 

    printf("decryption with bigra9m : \n") ; 
    
    memset(decryption_buf2 , 0 , 64) ; 
    before = clock() ; 
    rsa_decrypt(output2 , decryption_buf2 , 64 , &rsa_key2) ; 
    after = clock() ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    PRINT_ARRAY_NAI(decryption_buf2 , 64 , "%x") ; 


    // void *key = malloc(sizeof(RsaKey)) ; 
    // RsaKey *rsa_key ; 
    // rsa_set_key(key ,"10:10:10:10" , 10 ) ; 
    // printf("hi\n") ; 
    // rsa_key = (RsaKey*) key ; 
    // printf("hi\n") ; 
    // printf("") ; 
    // rsa_key->bits ; 
    // printf("hi after\n") ; 
    // mpz_t a ;

    // mpz_init(&a) ; 
    // mpz_set_str(&a , "123456789" , 10) ; 

    // gmp_printf("a : %Zd\n" , a) ; 
    // gmp_printf("bits : %d\n" , rsa_key->bits) ; 

    // gmp_printf("d : %Zd\n" , rsa_key->d) ; 
    // gmp_printf("e : %Zd\n" , rsa_key->e) ; 
    // gmp_printf("n : %Zd\n" , rsa_key->n) ; 


    // hybrid_test() ; 


    printf("Done\n");
    return 0;
}


