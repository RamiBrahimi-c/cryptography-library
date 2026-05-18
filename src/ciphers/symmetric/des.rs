use crate::cipher_interface::Cipher;
use cbc::{Decryptor, Encryptor};
use cipher::{block_padding::Pkcs7, BlockDecryptMut, BlockEncryptMut, KeyIvInit};
use des::Des;

#[derive(Clone, Debug)]
pub struct DesKey {
    pub raw: [u8; 8],
    pub iv: [u8; 8],
}

impl Default for DesKey {
    fn default() -> Self {
        Self {
            raw: *b"DESdemo!",
            iv: [0u8; 8],
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct DesCipher {
    pub key: DesKey,
}

impl Cipher for DesCipher {
    fn name(&self) -> &'static str {
        "des"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        Encryptor::<Des>::new_from_slices(&self.key.raw, &self.key.iv)
            .expect("valid DES params")
            .encrypt_padded_vec_mut::<Pkcs7>(input)
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        Decryptor::<Des>::new_from_slices(&self.key.raw, &self.key.iv)
            .expect("valid DES params")
            .decrypt_padded_vec_mut::<Pkcs7>(input)
            .unwrap_or_default()
    }
}
