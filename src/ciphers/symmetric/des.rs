use crate::cipher_interface::Cipher;
use super::reseau_fistel::encrypt_fistel;

#[derive(Clone, Debug)]
pub struct DesKey {
    pub raw: [u8; 8],
}

impl Default for DesKey {
    fn default() -> Self {
        Self { raw: *b"DESdemo!" }
    }
}

#[derive(Clone, Debug, Default)]
pub struct DesCipher {
    pub key: DesKey,
}

fn round_function(block: &[u8], key: &[u8; 8], round: usize) -> Vec<u8> {
    block
        .iter()
        .enumerate()
        .map(|(i, byte)| byte.rotate_left(((key[(i + round) % key.len()] % 7) + 1) as u32))
        .collect()
}

impl Cipher for DesCipher {
    fn name(&self) -> &'static str {
        "des"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        let mut block = input.to_vec();
        if block.len() % 2 == 1 {
            block.push(0);
        }
        encrypt_fistel(&block, 16, |half| round_function(half, &self.key.raw, 0))
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        encrypt_fistel(input, 16, |half| round_function(half, &self.key.raw, 0))
    }
}
