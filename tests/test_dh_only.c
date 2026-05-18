/* #include <stdio.h>
#include <gmp.h>
#include <string.h>
#include "dh.h"

int main()
{
    printf("=== Diffie-Hellman Key Exchange ===\n\n");

    // 1. Generate shared parameters
    mpz_t p, g;
    mpz_inits(p, g, NULL);
    dh_generate_params(p, g, 512);
    gmp_printf("Shared prime p = %Zd\n", p);
    gmp_printf("Generator g    = %Zd\n\n", g);

    // 2. Alice generates keypair
    DHParty alice;
    mpz_inits(alice.p, alice.private_key, alice.public_key, NULL);
    strcpy(alice.name, "Alice");
    dh_generate_keypair(&alice, p, g);
    gmp_printf("Alice public key  = %Zd\n\n", alice.public_key);

    // 3. Bob generates keypair
    DHParty bob;
    mpz_inits(bob.p, bob.private_key, bob.public_key, NULL);
    strcpy(bob.name, "Bob");
    dh_generate_keypair(&bob, p, g);
    gmp_printf("Bob public key    = %Zd\n\n", bob.public_key);

    // 4. Compute shared secrets
    mpz_t alice_shared, bob_shared;
    mpz_inits(alice_shared, bob_shared, NULL);

    dh_compute_shared(alice_shared, &alice, bob.public_key);
    dh_compute_shared(bob_shared, &bob, alice.public_key);

    gmp_printf("Alice computed K = %Zd\n", alice_shared);
    gmp_printf("Bob computed K   = %Zd\n", bob_shared);
    printf("Keys match? %s\n\n", mpz_cmp(alice_shared, bob_shared) == 0 ? "YES ✓" : "NO ✗");

    // ============================================================
    // MITM ATTACK
    // ============================================================
    printf("=== Man-in-the-Middle Attack ===\n\n");
    printf("ASCII Art:\n");
    printf("  Alice ────► g^a ────► Mallory ────► g^m1 ────► Bob\n");
    printf("  Alice ◄─── g^b ◄──── Mallory ◄──── g^m2 ◄──── Bob\n\n");

    // Mallory creates two keypairs
    DHParty mallory_for_alice;
    mpz_inits(mallory_for_alice.p, mallory_for_alice.private_key, mallory_for_alice.public_key, NULL);
    strcpy(mallory_for_alice.name, "Mallory->Alice");
    dh_generate_keypair(&mallory_for_alice, p, g);

    DHParty mallory_for_bob;
    mpz_inits(mallory_for_bob.p, mallory_for_bob.private_key, mallory_for_bob.public_key, NULL);
    strcpy(mallory_for_bob.name, "Mallory->Bob");
    dh_generate_keypair(&mallory_for_bob, p, g);

    gmp_printf("Mallory's public key for Alice = %Zd\n", mallory_for_alice.public_key);
    gmp_printf("Mallory's public key for Bob   = %Zd\n\n", mallory_for_bob.public_key);

    // Alice thinks she's talking to Bob, but receives Mallory's key
    // Bob thinks he's talking to Alice, but receives Mallory's other key
    mpz_t alice_fake_shared, bob_fake_shared, mallory_shared_with_alice, mallory_shared_with_bob;
    mpz_inits(alice_fake_shared, bob_fake_shared, mallory_shared_with_alice, mallory_shared_with_bob, NULL);

    // Alice computes shared with Mallory (thinks it's Bob)
    dh_compute_shared(alice_fake_shared, &alice, mallory_for_bob.public_key);
    // Bob computes shared with Mallory (thinks it's Alice)
    dh_compute_shared(bob_fake_shared, &bob, mallory_for_alice.public_key);
    // Mallory computes both
    dh_compute_shared(mallory_shared_with_alice, &mallory_for_alice, alice.public_key);
    dh_compute_shared(mallory_shared_with_bob, &mallory_for_bob, bob.public_key);

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
    dh_clear_party(&alice);
    dh_clear_party(&bob);
    dh_clear_party(&mallory_for_alice);
    dh_clear_party(&mallory_for_bob);

    printf("=== Done ===\n");
    return 0;
} */