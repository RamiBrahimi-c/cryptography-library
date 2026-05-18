#include "dh.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void dh_generate_params(mpz_t p, mpz_t g, int bits)
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

void dh_generate_keypair(DHParty* party, const mpz_t p, const mpz_t g)
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



void dh_compute_shared(mpz_t shared, const DHParty* me, const mpz_t their_public)
{
    mpz_powm(shared, their_public, me->private_key, me->p);
}



void dh_clear_party(DHParty* party)
{
    mpz_clears(party->p, party->private_key, party->public_key, NULL);
}