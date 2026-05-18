use crate::cipher_interface::Cipher;
use crate::common::utils::mod_inverse;

#[derive(Clone, Debug)]
pub struct AffineKey {
    pub a: i16,
    pub b: i16,
}

impl Default for AffineKey {
    fn default() -> Self {
        Self { a: 5, b: 8 }
    }
}

#[derive(Clone, Debug, Default)]
pub struct AffineCipher {
    pub key: AffineKey,
}

fn encode(byte: u8, key: &AffineKey, decrypt: bool) -> u8 {
    if !byte.is_ascii_alphabetic() {
        return byte;
    }
    let base = if byte.is_ascii_lowercase() { b'a' } else { b'A' };
    let value = (byte - base) as i16;
    let mapped = if decrypt {
        let inv = mod_inverse(key.a as i64, 26).unwrap_or(1) as i16;
        inv * (value - key.b)
    } else {
        key.a * value + key.b
    };
    mapped.rem_euclid(26) as u8 + base
}

impl Cipher for AffineCipher {
    fn name(&self) -> &'static str {
        "affine"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| encode(b, &self.key, false)).collect()
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| encode(b, &self.key, true)).collect()
    }
}
