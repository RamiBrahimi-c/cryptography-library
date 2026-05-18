use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct SubstitutionKey {
    pub forward: [u8; 26],
    pub backward: [u8; 26],
}

impl Default for SubstitutionKey {
    fn default() -> Self {
        let forward = *b"QWERTYUIOPASDFGHJKLZXCVBNM";
        let mut backward = [0u8; 26];
        for (idx, mapped) in forward.iter().enumerate() {
            backward[(mapped - b'A') as usize] = b'A' + idx as u8;
        }
        Self { forward, backward }
    }
}

#[derive(Clone, Debug, Default)]
pub struct SubstitutionCipher {
    pub key: SubstitutionKey,
}

fn transform(byte: u8, table: &[u8; 26]) -> u8 {
    if byte.is_ascii_uppercase() {
        table[(byte - b'A') as usize]
    } else if byte.is_ascii_lowercase() {
        table[(byte.to_ascii_uppercase() - b'A') as usize].to_ascii_lowercase()
    } else {
        byte
    }
}

impl Cipher for SubstitutionCipher {
    fn name(&self) -> &'static str {
        "substitution"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| transform(b, &self.key.forward)).collect()
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        input.iter().map(|&b| transform(b, &self.key.backward)).collect()
    }
}
