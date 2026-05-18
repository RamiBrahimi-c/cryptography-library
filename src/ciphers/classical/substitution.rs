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

impl SubstitutionKey {
    pub fn from_key(key: &str) -> Option<Self> {
        let clean = key.as_bytes();
        if clean.len() != 26 {
            return None;
        }
        let mut seen = [false; 26];
        let mut forward = [0u8; 26];
        let mut backward = [0u8; 26];
        for (idx, byte) in clean.iter().copied().enumerate() {
            let upper = byte.to_ascii_uppercase();
            if !upper.is_ascii_uppercase() {
                return None;
            }
            let slot = (upper - b'A') as usize;
            if seen[slot] {
                return None;
            }
            seen[slot] = true;
            forward[idx] = upper;
            backward[slot] = b'A' + idx as u8;
        }
        Some(Self { forward, backward })
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
