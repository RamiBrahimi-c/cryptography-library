#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include "dh.h"


typedef struct {
    mpz_t p;             // shared prime 
    mpz_t private_key;
    mpz_t public_key;
    char name[32];
} DHPartyGmp;


static void gmp_dh_generate_params(mpz_t p, mpz_t g, int bits);
static void gmp_dh_generate_keypair(DHPartyGmp* party, const mpz_t p, const mpz_t g);
static void gmp_dh_compute_shared(mpz_t shared, const DHPartyGmp* me, const mpz_t their_public);
static void gmp_dh_clear_party(DHPartyGmp* party);

#define COMPARE_GMP_DH 0

int main()
{
    printf("=== Diffie-Hellman Key Exchange ===\n\n");

    // 1. Generate shared parameters
    #if COMPARE_GMP_DH
    mpz_t p, g;
    mpz_inits(p, g, NULL);
    gmp_dh_generate_params(p, g, 64);
    gmp_printf("Shared prime p = %Zd\n", p);
    gmp_printf("Generator g    = %Zd\n\n", g);
    #endif

    BigInt p1, g1;
    bigra9m_inits(&p1, &g1, NULL);
    dh_generate_params(&p1 , &g1 , 64) ; 
    printf("Shared prime p = ");
    bigra9m_print(p1) ; 
    printf("Generator g = ");
    bigra9m_print(g1) ; 
    
    printf("\n\n\n\n") ; 
    // 2. Alice generates keypair
    #if COMPARE_GMP_DH
    DHPartyGmp alice;
    mpz_inits(alice.p, alice.private_key, alice.public_key, NULL);
    strcpy(alice.name, "Alice");
    gmp_dh_generate_keypair(&alice, p, g);
    gmp_printf("Alice public key  = %Zd\n\n", alice.public_key);
    #endif

    DHParty real_alice;
    bigra9m_inits(&real_alice.p, &real_alice.private_key, &real_alice.public_key, NULL);
    strcpy(real_alice.name, "real_alice");
    dh_generate_keypair(&real_alice, &p1, &g1);
    printf("real_alice public key  = ");
    bigra9m_print(real_alice.public_key) ; 
    
    
    // 3. Bob generates keypair
    #if COMPARE_GMP_DH
    DHPartyGmp bob;
    mpz_inits(bob.p, bob.private_key, bob.public_key, NULL);
    strcpy(bob.name, "Bob");
    gmp_dh_generate_keypair(&bob, p, g);
    gmp_printf("Bob public key    = %Zd\n\n", bob.public_key);
    #endif
    
    DHParty real_bob;
    bigra9m_inits(&real_bob.p, &real_bob.private_key, &real_bob.public_key, NULL);
    strcpy(real_bob.name, "real_bob");
    dh_generate_keypair(&real_bob, &p1, &g1);
    printf("real_bob public key  = ");
    bigra9m_print(real_bob.public_key) ; 
    
    
    
    
    // 4. Compute shared secrets
    #if COMPARE_GMP_DH
    mpz_t alice_shared, bob_shared;
    mpz_inits(alice_shared, bob_shared, NULL);
    
    gmp_dh_compute_shared(alice_shared, &alice, bob.public_key);
    gmp_dh_compute_shared(bob_shared, &bob, alice.public_key);
    
    gmp_printf("Alice computed K = %Zd\n", alice_shared);
    gmp_printf("Bob computed K   = %Zd\n", bob_shared);
    printf("Keys match? %s\n\n", mpz_cmp(alice_shared, bob_shared) == 0 ? "YES ✓" : "NO ✗");
    #endif


    BigRa9m real_alice_shared, real_bob_shared;
    bigra9m_inits(&real_alice_shared, &real_bob_shared, NULL);
    
    dh_compute_shared(&real_alice_shared, &real_alice, &real_bob.public_key);
    dh_compute_shared(&real_bob_shared, &real_bob, &real_alice.public_key);
    
    printf("Alice computed K = ");
    bigra9m_print(real_alice_shared) ;
    printf("Bob computed K   = ");
    bigra9m_print(real_bob_shared) ;
    printf("Keys match? %s\n\n", bigra9m_isEqualNum(real_alice_shared, real_bob_shared)  ? "YES ✓" : "NO ✗");
    
    
    
    // _exit(0) ;


    // ============================================================
    // MITM ATTACK
    // ============================================================
    
    
    printf("=== Man-in-the-Middle Attack ===\n\n");
    printf("ASCII Art:\n");
    printf("  Alice ────► g^a ────► Mallory ────► g^m1 ────► Bob\n");
    printf("  Alice ◄─── g^b ◄──── Mallory ◄──── g^m2 ◄──── Bob\n\n");

    #if COMPARE_GMP_DH
    // Mallory creates two keypairs
    DHPartyGmp mallory_for_alice;
    mpz_inits(mallory_for_alice.p, mallory_for_alice.private_key, mallory_for_alice.public_key, NULL);
    strcpy(mallory_for_alice.name, "Mallory->Alice");
    gmp_dh_generate_keypair(&mallory_for_alice, p, g);

    DHPartyGmp mallory_for_bob;
    mpz_inits(mallory_for_bob.p, mallory_for_bob.private_key, mallory_for_bob.public_key, NULL);
    strcpy(mallory_for_bob.name, "Mallory->Bob");
    gmp_dh_generate_keypair(&mallory_for_bob, p, g);

    gmp_printf("Mallory's public key for Alice = %Zd\n", mallory_for_alice.public_key);
    gmp_printf("Mallory's public key for Bob   = %Zd\n\n", mallory_for_bob.public_key);

    // Alice thinks she's talking to Bob, but receives Mallory's key
    // Bob thinks he's talking to Alice, but receives Mallory's other key
    mpz_t alice_fake_shared, bob_fake_shared, mallory_shared_with_alice, mallory_shared_with_bob;
    mpz_inits(alice_fake_shared, bob_fake_shared, mallory_shared_with_alice, mallory_shared_with_bob, NULL);

    // Alice computes shared with Mallory (thinks it's Bob)
    gmp_dh_compute_shared(alice_fake_shared, &alice, mallory_for_bob.public_key);
    // Bob computes shared with Mallory (thinks it's Alice)
    gmp_dh_compute_shared(bob_fake_shared, &bob, mallory_for_alice.public_key);
    // Mallory computes both
    gmp_dh_compute_shared(mallory_shared_with_alice, &mallory_for_alice, alice.public_key);
    gmp_dh_compute_shared(mallory_shared_with_bob, &mallory_for_bob, bob.public_key);

    gmp_printf("Alice computes K (with Mallory) = %Zd\n", alice_fake_shared);
    gmp_printf("Bob computes K (with Mallory)   = %Zd\n", bob_fake_shared);
    gmp_printf("Mallory's K with Alice          = %Zd\n", mallory_shared_with_alice);
    gmp_printf("Mallory's K with Bob            = %Zd\n\n", mallory_shared_with_bob);

    printf("Alice and Bob's keys match? %s\n",
           mpz_cmp(alice_fake_shared, bob_fake_shared) == 0 ? "YES" : "NO (different!)");
    printf("Mallory can read all traffic: decrypt from Alice with K1, re-encrypt to Bob with K2.\n\n");

    // Cleanup
    mpz_clears(p, g, alice_shared, bob_shared, NULL);
    mpz_clears(alice_fake_shared, bob_fake_shared, mallory_shared_with_alice, mallory_shared_with_bob, NULL);
    #endif

    // dh_clear_party(&alice);
    // dh_clear_party(&bob);
    // dh_clear_party(&mallory_for_alice);
    // dh_clear_party(&mallory_for_bob);

    printf("=== Done ===\n");
    return 0;
}


// Reuse prime generation from ElGamal
static void generate_prime(mpz_t p, int bits)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    unsigned long seed = time(NULL);
    if (seed == 0) seed = 42;
    gmp_randseed_ui(state, seed);

    mpz_urandomb(p, state, bits);
    mpz_setbit(p, bits - 1);
    mpz_nextprime(p, p);

    gmp_randclear(state);
}


static void gmp_dh_generate_params(mpz_t p, mpz_t g, int bits)
{
    // Generate large prime
    generate_prime(p, bits);

    // Generator: use 2 or 5
    mpz_t p_minus_1, q, test;
    mpz_inits(p_minus_1, q, test, NULL);
    mpz_sub_ui(p_minus_1, p, 1);

    unsigned long candidates[] = {2, 3, 5, 7, 11, 13};
    int found = 0;
    for (int i = 0; i < 6; i++) {
        mpz_set_ui(g, candidates[i]);
        mpz_tdiv_q_ui(q, p_minus_1, 2);
        mpz_powm(test, g, q, p);
        if (mpz_cmp_ui(test, 1) != 0) {
            found = 1;
            break;
        }
    }
    if (!found) mpz_set_ui(g, 2);

    mpz_clears(p_minus_1, q, test, NULL);
}

static void gmp_dh_generate_keypair(DHPartyGmp* party, const mpz_t p, const mpz_t g)
{
    mpz_set(party->p, p);   // store p
    
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL) ^ rand());

    mpz_t p_minus_1;
    mpz_init(p_minus_1);
    mpz_sub_ui(p_minus_1, p, 1);

    mpz_urandomm(party->private_key, state, p_minus_1);
    if (mpz_cmp_ui(party->private_key, 0) == 0)
        mpz_set_ui(party->private_key, 1);

    mpz_powm(party->public_key, g, party->private_key, p);

    mpz_clear(p_minus_1);
    gmp_randclear(state);
}



static void gmp_dh_compute_shared(mpz_t shared, const DHPartyGmp* me, const mpz_t their_public)
{
    mpz_powm(shared, their_public, me->private_key, me->p);
}



static void gmp_dh_clear_party(DHPartyGmp* party)
{
    mpz_clears(party->p, party->private_key, party->public_key, NULL);
}


