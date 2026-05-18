#ifndef DH_H
#define DH_H

#include <gmp.h>

typedef struct {
    mpz_t p;             // shared prime 
    mpz_t private_key;
    mpz_t public_key;
    char name[32];
} DHParty;



void dh_generate_params(mpz_t p, mpz_t g, int bits);
void dh_generate_keypair(DHParty* party, const mpz_t p, const mpz_t g);
void dh_compute_shared(mpz_t shared, const DHParty* me, const mpz_t their_public);
void dh_clear_party(DHParty* party);

#endif