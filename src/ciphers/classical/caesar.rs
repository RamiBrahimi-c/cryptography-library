use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct CaesarCipher {
    pub shift: u8,
}

impl Default for CaesarCipher {
    fn default() -> Self {
        Self { shift: 5 }
    }
}

fn shift_ascii(byte: u8, shift: i16) -> u8 {
    if byte.is_ascii_alphabetic() {
        let base = if byte.is_ascii_lowercase() { b'a' } else { b'A' };
        (((byte - base) as i16 + shift).rem_euclid(26) as u8) + base
    } else {
        byte
    }
}

impl Cipher for CaesarCipher {
    fn name(&self) -> &'static str {
        "caesar"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| shift_ascii(b, self.shift as i16)).collect()
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| shift_ascii(b, -(self.shift as i16))).collect()
    }
}

pub fn encrypt_caesar(src: &[u8]) -> Vec<u8> {
    CaesarCipher::default().encrypt(src)
}
