use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct VigenereCipher {
    pub key: Vec<u8>,
}

impl VigenereCipher {
    pub fn new(key: impl AsRef<[u8]>) -> Self {
        let mut clean = key.as_ref().to_ascii_uppercase();
        if clean.is_empty() {
            clean.push(b'K');
        }
        Self { key: clean }
    }
}

fn apply(input: &[u8], key: &[u8], decrypt: bool) -> Vec<u8> {
    input
        .iter()
        .enumerate()
        .map(|(i, &byte)| {
            if !byte.is_ascii_alphabetic() {
                return byte;
            }
            let base = if byte.is_ascii_lowercase() { b'a' } else { b'A' };
            let step = (key[i % key.len()].to_ascii_uppercase() - b'A') as i16;
            let signed = if decrypt { -step } else { step };
            (((byte - base) as i16 + signed).rem_euclid(26) as u8) + base
        })
        .collect()
}

impl Cipher for VigenereCipher {
    fn name(&self) -> &'static str {
        "vigenere"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        apply(input, &self.key, false)
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        apply(input, &self.key, true)
    }
}

pub fn encrypt_vigenere(src: &[u8]) -> Vec<u8> {
    VigenereCipher::new("KEY").encrypt(src)
}
