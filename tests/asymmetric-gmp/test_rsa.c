#include <stdio.h>
#include <string.h>
#include <gmp.h> // only cuz we are testing side to side !!!!!!
#include "rsa.h"
#include "aes.h"

#include "bigra9m.h"



typedef struct {
    mpz_t n;      // modulus
    mpz_t e;      // public exponent
    mpz_t d;      // private exponent (only for private key)
    int   bits;   // key size (512, 1024, 2048)
    int   is_private;   // 1 if we have d, 0 if only public
} RsaKeyGmp;



static void gmp_rsa_generate_keypair(RsaKeyGmp* key, int bits, unsigned long e_value) ;
static void gmp_rsa_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key) ;
static void gmp_rsa_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key) ;
static void gmp_rsa_set_key(void* key_struct, const char* key_str , size_t key_len) ;
static void gmp_rsa_free_key(void* key_struct) ; 
static void gmp_rsa_export_key(const RsaKeyGmp* key) ;
static void gmp_test_rsa_32bytes(int bits) ;
static void gmp_hybrid_test(void) ;



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
    RsaKeyGmp  rsa_key1 ; 
    RsaKey rsa_key2  ; 
    clock_t before , after ;

    
    bigra9m_inits(&rsa_key2.d ,&rsa_key2.e , &rsa_key2.n , NULL ) ;
    
    int bits = 512 ; 
    printf("========================= key 1 =================================\n") ; 
    before = clock() ; 
    rsa_generate_keypair(&rsa_key2 , bits , 13793) ; 
    after = clock() ; 
    
    rsa_export_key(&rsa_key2 ) ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    
    printf("========================= key 2 =================================\n") ; 
    mpz_inits(rsa_key1.d , rsa_key1.e , rsa_key1.n , NULL ) ; 
    // rsa_key1.is_private = 1 ; 
    before = clock() ; 
    gmp_rsa_generate_keypair(&rsa_key1 , bits , 13793) ; 
    after = clock() ; 
    gmp_rsa_export_key(&rsa_key1) ; 
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
    printf("enc with gmp : \n") ; 
    
    before = clock() ; 
    gmp_rsa_encrypt(input , output , input_len , &rsa_key1) ; 
    after = clock() ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    
    PRINT_ARRAY_NAI(input , input_len , "%x") ; 
    PRINT_ARRAY_NAI(output , 64 , "%x") ; 
    
    printf("decryption with gmp : \n") ; 
    
    before = clock() ; 
    gmp_rsa_decrypt(output , decryption_buf , 64 , &rsa_key1) ; 
    after = clock() ; 
    printf("took : %f \n" ,(double) (after - before) / CLOCKS_PER_SEC ) ; 
    PRINT_ARRAY_NAI(decryption_buf , 64 , "%x") ; 
    
    
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





static void gmp_rsa_generate_keypair(RsaKeyGmp* key, int bits, unsigned long e_value)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    unsigned long seed = time(NULL);
    if (seed == 0) seed = 42;
    gmp_randseed_ui(state, seed);

    mpz_t p, q, n, phi, p1, q1, gcd;
    mpz_inits(p, q, n, phi, p1, q1, gcd, NULL);

    int prime_bits = bits / 2;

    // Generate p
    mpz_urandomb(p, state, prime_bits);
    mpz_setbit(p, prime_bits - 1);  // ensure exactly prime_bits
    mpz_nextprime(p, p);
    
    // Generate q (different from p)
    do {
        mpz_urandomb(q, state, prime_bits);
        mpz_setbit(q, prime_bits - 1);
        mpz_nextprime(q, q);
    } while (mpz_cmp(p, q) == 0);
    
    // n = p * q
    mpz_mul(key->n, p, q);
    
    // phi = (p-1)*(q-1)
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);
    
    // e
    printf("wtf\n") ; 
    mpz_set_ui(key->e, e_value);
    printf("wtf\n") ; 
    mpz_gcd(gcd, key->e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        mpz_set_ui(key->e, 65537);
    }

    // d = e⁻¹ mod phi
    mpz_invert(key->d, key->e, phi);

    key->bits = bits;
    key->is_private = 1;

    mpz_clears(p, q, n, phi, p1, q1, gcd, NULL);
    gmp_randclear(state);
}


static void gmp_rsa_encrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    printf("noooo\n") ; 
    const RsaKeyGmp* rsa = (const RsaKeyGmp*)key;
    size_t out_bytes = (rsa->bits + 7) / 8;
    memset(output, 0, out_bytes);
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    mpz_import(m, length, 1, 1, 0, 0, input);
    gmp_printf("m : %Zu \n" , m) ; 
    mpz_powm(c, m, rsa->e, rsa->n);
    gmp_printf("c : %Zu \n" , c) ; 

    // Export to temporary buffer, then right-align into output
    size_t count = 0;
    unsigned char temp[1024];
    mpz_export(temp, &count, 1, 1, 0, 0, c);
    printf("out bytes : %ld \n" , out_bytes) ; 
    printf("count : %ld \n" , count) ; 
    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }

    mpz_clears(m, c, NULL);
}


static void gmp_rsa_decrypt(const uchar_t* input, uchar_t* output, int length, const void* key)
{
    const RsaKeyGmp* rsa = (const RsaKeyGmp*)key;
    if (!rsa || !rsa->is_private || length <= 0) return;
    size_t out_bytes = (rsa->bits + 7) / 8;
    
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    mpz_import(c, length, 1, 1, 0, 0, input);
    gmp_printf("c : %Zu \n" , c) ;     
    
    mpz_powm(m, c, rsa->d, rsa->n);
    gmp_printf("m : %Zu \n" , m) ;     
    
    // Export to temp, then copy right‑aligned into output buffer
    unsigned char temp[1024];   // up to 2048 bits = 256 bytes, safe
    size_t count = 0;
    mpz_export(temp, &count, 1, 1, 0, 0, m);
    
    // Zero the whole output buffer
    memset(output, 0, out_bytes);

    // Right‑align the actual bytes
    if (count <= out_bytes) {
        memcpy(output + (out_bytes - count), temp, count);
    } else {
        // Should not happen, but just copy the last out_bytes bytes
        memcpy(output, temp + (count - out_bytes), out_bytes);
    }
    
    printf("fuck you\n") ; 
    mpz_clears(c, m, NULL);
}



static void gmp_rsa_set_key(void* key_struct, const char* key_str , size_t key_len)
{
    RsaKeyGmp* rsa = (RsaKeyGmp*)key_struct;
    // parse "n_hex:e_hex:d_hex:bits"
    char* copy = strdup(key_str);
    char* tok_n = strtok(copy, ":");
    char* tok_e = strtok(NULL, ":");
    char* tok_d = strtok(NULL, ":");
    char* tok_bits = strtok(NULL, ":");
    if (tok_n && tok_e && tok_d && tok_bits) {
        mpz_set_str(rsa->n, tok_n, 16);
        mpz_set_str(rsa->e, tok_e, 16);
        mpz_set_str(rsa->d, tok_d, 16);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 1;
        printf("wth1\n") ; 
    } else if (tok_n && tok_e && tok_bits) {
        mpz_set_str(rsa->n, tok_n, 16);
        mpz_set_str(rsa->e, tok_e, 16);
        rsa->bits = atoi(tok_bits);
        rsa->is_private = 0;
        printf("wth2\n") ; 
    } else {
        printf("wth\n") ; 
    }
    free(copy);
}



static void gmp_rsa_free_key(void* key_struct)
{
    RsaKeyGmp* rsa = (RsaKeyGmp*)key_struct;
    mpz_clears(rsa->n, rsa->e, rsa->d, NULL);
}



static void gmp_rsa_export_key(const RsaKeyGmp* key) {
    printf("Modulus n (hex): ");
    mpz_out_str(stdout, 10, key->n);
    printf("\nPublic exponent e (hex): ");
    mpz_out_str(stdout, 10, key->e);
    if (key->is_private) {
        printf("\nPrivate exponent d (hex): ");
        mpz_out_str(stdout, 10, key->d);
    }
    printf("\nBits: %d\n", key->bits);
}



static void gmp_test_rsa_32bytes(int bits) {
    printf("testing with %d bits\n", bits);
    fflush(stdout);

    RsaKeyGmp key;
    mpz_inits(key.n, key.e, key.d, NULL);

    rsa_generate_keypair(&key, bits, 65537);
    printf("generated key pairs successfully for %d bits\n", bits);
    fflush(stdout);

    uchar_t plain[32];
    for (int i = 0; i < 32; i++) plain[i] = (uchar_t)(i + 0x41);

    size_t out_bytes = (bits + 7) / 8;
    uchar_t* cipher = malloc(out_bytes);
    uchar_t* decrypted = malloc(out_bytes);

    rsa_encrypt(plain, cipher, 32, &key);
    rsa_decrypt(cipher, decrypted, out_bytes, &key);

    uchar_t recovered[32];
    memcpy(recovered, decrypted + (out_bytes - 32), 32);

    printf("RSA-%d match: %s\n", bits,
           memcmp(plain, recovered, 32) == 0 ? "YES" : "NO");
    fflush(stdout);

    free(cipher);
    free(decrypted);
    rsa_free_key(&key);
}

// test_rsa_32bytes(512), test_rsa_32bytes(1024), test_rsa_32bytes(2048)


static void gmp_hybrid_test(void) {
    printf("hi") ;
    RsaKeyGmp rsa;
    mpz_inits(rsa.n, rsa.e, rsa.d, NULL);
    rsa_generate_keypair(&rsa, 2048, 65537);
    
    // 1. Random AES‑256 key (32 bytes)
    uchar_t aes_key[32];
    for (int i = 0; i < 32; i++) aes_key[i] = rand() & 0xFF;   // weak randomness
    
    // 2. Encrypt AES key with RSA
    size_t rsa_out = (rsa.bits + 7) / 8;
    uchar_t encrypted_key[rsa_out];
    clock_t t1 = clock();
    printf("hi\n") ;
    rsa_encrypt(aes_key,  encrypted_key , 32, &rsa);
    printf("hi!!!!!!!!!!!\n") ;
    clock_t t2 = clock();
    double rsa_time = (double)(t2 - t1) / CLOCKS_PER_SEC;

    // 3. Read 1 MB file (or create a dummy 1 MB buffer)
    size_t file_size = 1024 * 1024;
    uchar_t* file_data = malloc(file_size);
    memset(file_data, 'A', file_size);   // dummy content

    // 4. Encrypt with AES 
    AesKey aes_ctx;   // assuming AES struct
    aes_set_key(&aes_ctx, (const uchar_t*) aes_key , 32);

    size_t padded_size = file_size + (16 - (file_size % 16));   // PKCS#7 padding
    uchar_t* aes_cipher = malloc(padded_size);
    clock_t t3 = clock();
    aes_encrypt(file_data, aes_cipher, file_size, &aes_ctx);   // this will write padded_size bytes
    clock_t t4 = clock();
    double aes_time = (double)(t4 - t3) / CLOCKS_PER_SEC;

    printf("RSA key encryption time: %f s\n", rsa_time);
    printf("AES file encryption time: %f s\n", aes_time);

    free(file_data);
    free(aes_cipher);
    rsa_free_key(&rsa);
}


