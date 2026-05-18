use crate::common::utils::{biguint_gcd, biguint_mod_inverse, generate_probable_prime};
use num_bigint::BigUint;
use num_traits::One;

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

fn left_pad(mut input: Vec<u8>, size: usize) -> Vec<u8> {
    if input.len() >= size {
        return input;
    }
    let mut out = vec![0u8; size - input.len()];
    out.append(&mut input);
    out
}

pub fn rsa_generate_keypair(bits: usize, e_value: u64) -> RsaKey {
    let p = generate_probable_prime((bits / 2).max(32));
    let mut q = generate_probable_prime((bits / 2).max(32));
    while q == p {
        q = generate_probable_prime((bits / 2).max(32));
    }
    let n = &p * &q;
    let phi = (&p - BigUint::one()) * (&q - BigUint::one());
    let mut e = BigUint::from(e_value);
    while biguint_gcd(e.clone(), phi.clone()) != BigUint::one() {
        e += BigUint::from(2u32);
    }
    let d = biguint_mod_inverse(&e, &phi).unwrap_or_else(BigUint::one);
    RsaKey {
        n,
        e,
        d,
        bits,
        is_private: true,
    }
}

pub fn rsa_encrypt(input: &[u8], key: &RsaKey) -> Vec<u8> {
    let cipher_block = ((key.n.bits() + 7) / 8) as usize;
    let plain_block = cipher_block.saturating_sub(1).max(3);
    let data_size = plain_block - 2;
    let mut out = Vec::new();
    for chunk in input.chunks(data_size) {
        let mut block = vec![0u8; plain_block];
        let len = chunk.len() as u16;
        block[0..2].copy_from_slice(&len.to_be_bytes());
        block[2..2 + chunk.len()].copy_from_slice(chunk);
        let m = BigUint::from_bytes_be(&block);
        let c = m.modpow(&key.e, &key.n);
        out.extend_from_slice(&left_pad(c.to_bytes_be(), cipher_block));
    }
    out
}

pub fn rsa_decrypt(input: &[u8], key: &RsaKey) -> Vec<u8> {
    let cipher_block = ((key.n.bits() + 7) / 8) as usize;
    let plain_block = cipher_block.saturating_sub(1).max(3);
    let mut out = Vec::new();
    for chunk in input.chunks(cipher_block) {
        let c = BigUint::from_bytes_be(chunk);
        let m = c.modpow(&key.d, &key.n);
        let block = left_pad(m.to_bytes_be(), plain_block);
        let len = u16::from_be_bytes([block[0], block[1]]) as usize;
        let end = 2 + len.min(block.len().saturating_sub(2));
        out.extend_from_slice(&block[2..end]);
    }
    out
}

pub fn rsa_export_key(key: &RsaKey) -> String {
    format!("n={:x}\ne={:x}\nd={:x}\nbits={}", key.n, key.e, key.d, key.bits)
}
