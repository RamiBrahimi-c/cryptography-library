use crate::common::utils::{biguint_mod_inverse, generate_probable_prime};
use num_bigint::{BigUint, RandBigInt};
use num_traits::One;
use rand::thread_rng;

#[derive(Clone, Debug)]
pub struct ElGamalKey {
    pub p: BigUint,
    pub g: BigUint,
    pub x: BigUint,
    pub h: BigUint,
}

#[derive(Clone, Debug)]
pub struct ElGamalCiphertext {
    pub c1: BigUint,
    pub c2: BigUint,
}

pub fn elgamal_generate_keypair(bits: usize) -> ElGamalKey {
    let p = generate_probable_prime(bits.max(64));
    let g = BigUint::from(7u32);
    let x = thread_rng().gen_biguint_range(&BigUint::from(2u32), &(&p - BigUint::from(2u32)));
    let h = g.modpow(&x, &p);
    ElGamalKey { p, g, x, h }
}

pub fn elgamal_encrypt(message: &BigUint, key: &ElGamalKey) -> ElGamalCiphertext {
    let y = thread_rng().gen_biguint_range(&BigUint::from(2u32), &(&key.p - BigUint::from(2u32)));
    let c1 = key.g.modpow(&y, &key.p);
    let s = key.h.modpow(&y, &key.p);
    let c2 = (message * s) % &key.p;
    ElGamalCiphertext { c1, c2 }
}

pub fn elgamal_decrypt(ct: &ElGamalCiphertext, key: &ElGamalKey) -> BigUint {
    let s = ct.c1.modpow(&key.x, &key.p);
    let inv = biguint_mod_inverse(&s, &key.p).unwrap_or_else(BigUint::one);
    (&ct.c2 * inv) % &key.p
}
