use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct Rc4Key {
    pub key: Vec<u8>,
}

impl Default for Rc4Key {
    fn default() -> Self {
        Self { key: b"rc4-demo-key".to_vec() }
    }
}

#[derive(Clone, Debug, Default)]
pub struct Rc4Cipher {
    pub key: Rc4Key,
}

fn rc4_stream(input: &[u8], key: &[u8]) -> Vec<u8> {
    let mut s = [0u8; 256];
    for (i, slot) in s.iter_mut().enumerate() {
        *slot = i as u8;
    }
    let mut j = 0usize;
    for i in 0..256 {
        j = (j + s[i] as usize + key[i % key.len()] as usize) % 256;
        s.swap(i, j);
    }
    let (mut i, mut j) = (0usize, 0usize);
    let mut out = Vec::with_capacity(input.len());
    for byte in input {
        i = (i + 1) % 256;
        j = (j + s[i] as usize) % 256;
        s.swap(i, j);
        let idx = (s[i] as usize + s[j] as usize) % 256;
        out.push(byte ^ s[idx]);
    }
    out
}

impl Cipher for Rc4Cipher {
    fn name(&self) -> &'static str {
        "rc4"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        rc4_stream(input, &self.key.key)
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        rc4_stream(input, &self.key.key)
    }
}
