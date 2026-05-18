use num_bigint::{BigUint, RandBigInt};
use num_traits::One;
use rand::thread_rng;

#[derive(Clone, Debug)]
pub struct DhParty {
    pub p: BigUint,
    pub g: BigUint,
    pub private: BigUint,
    pub public: BigUint,
}

pub fn dh_generate_params(_bits: usize) -> (BigUint, BigUint) {
    (BigUint::from(2_147_483_647u64), BigUint::from(5u32))
}

pub fn dh_generate_keypair(p: &BigUint, g: &BigUint) -> DhParty {
    let mut rng = thread_rng();
    let private = rng.gen_biguint(64) + BigUint::one();
    let public = g.modpow(&private, p);
    DhParty {
        p: p.clone(),
        g: g.clone(),
        private,
        public,
    }
}

pub fn dh_compute_shared(me: &DhParty, their_public: &BigUint) -> BigUint {
    their_public.modpow(&me.private, &me.p)
}
