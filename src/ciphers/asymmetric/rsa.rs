use num_bigint::{BigInt, BigUint, ToBigInt};
use num_traits::{One, Zero};
use rand::{thread_rng, Rng};

#[derive(Clone, Debug)]
pub struct RsaKey {
    pub n: BigUint,
    pub e: BigUint,
    pub d: BigUint,
    pub bits: usize,
    pub is_private: bool,
}

impl Default for RsaKey {
    fn default() -> Self {
        rsa_generate_keypair(512, 65_537)
    }
}

fn small_prime(bits: usize) -> u64 {
    let mut rng = thread_rng();
    let start = (1u64 << bits.min(30).saturating_sub(1)).max(257);
    let mut candidate = (start | 1) + (rng.gen::<u16>() as u64);
    loop {
        if is_prime(candidate) {
            return candidate;
        }
        candidate += 2;
    }
}

fn is_prime(n: u64) -> bool {
    if n < 2 {
        return false;
    }
    if n % 2 == 0 {
        return n == 2;
    }
    let mut i = 3;
    while i * i <= n {
        if n % i == 0 {
            return false;
        }
        i += 2;
    }
    true
}

fn egcd(a: BigInt, b: BigInt) -> (BigInt, BigInt, BigInt) {
    if b.is_zero() {
        (a, BigInt::one(), BigInt::zero())
    } else {
        let (g, x, y) = egcd(b.clone(), a.clone() % b.clone());
        (g, y.clone(), x - (a / b) * y)
    }
}

fn mod_inverse(a: &BigUint, m: &BigUint) -> BigUint {
    let (g, x, _) = egcd(a.to_bigint().unwrap(), m.to_bigint().unwrap());
    if g != BigInt::one() {
        return BigUint::one();
    }
    let modulus = m.to_bigint().unwrap();
    ((x % &modulus + &modulus) % &modulus).try_into().unwrap_or_else(|_| BigUint::one())
}

pub fn rsa_generate_keypair(bits: usize, e_value: u64) -> RsaKey {
    let p = BigUint::from(small_prime(bits / 2));
    let q = BigUint::from(small_prime(bits / 2 + 1));
    let n = &p * &q;
    let phi = (&p - BigUint::one()) * (&q - BigUint::one());
    let e = BigUint::from(e_value);
    let d = mod_inverse(&e, &phi);
    RsaKey { n, e, d, bits, is_private: true }
}

pub fn rsa_encrypt(input: &[u8], key: &RsaKey) -> Vec<u8> {
    let m = BigUint::from_bytes_be(input);
    m.modpow(&key.e, &key.n).to_bytes_be()
}

pub fn rsa_decrypt(input: &[u8], key: &RsaKey) -> Vec<u8> {
    let c = BigUint::from_bytes_be(input);
    c.modpow(&key.d, &key.n).to_bytes_be()
}

pub fn rsa_export_key(key: &RsaKey) -> String {
    format!("n={:x}\ne={:x}\nd={:x}\nbits={}", key.n, key.e, key.d, key.bits)
}
