use crate::cipher_interface::Cipher;
use aes::{Aes128, Aes192, Aes256};
use cbc::{Decryptor, Encryptor};
use cipher::{block_padding::Pkcs7, BlockDecryptMut, BlockEncryptMut, KeyIvInit};
use super::aes_lib::{aes_init_ctx_iv, AesCtx};

#[derive(Clone, Debug)]
pub struct AesKey {
    pub key_len: usize,
    pub key_bytes: Vec<u8>,
    pub ctx: AesCtx,
    pub iv: [u8; 16],
}

impl AesKey {
    pub fn new(key: impl AsRef<[u8]>) -> Self {
        let key_bytes = normalize_key(key.as_ref());
        let iv = [0u8; 16];
        let mut ctx = AesCtx::default();
        aes_init_ctx_iv(&mut ctx, &key_bytes, &iv);
        Self {
            key_len: key_bytes.len(),
            key_bytes,
            ctx,
            iv,
        }
    }
}

impl Default for AesKey {
    fn default() -> Self {
        Self::new(b"0123456789abcdef")
    }
}

#[derive(Clone, Debug, Default)]
pub struct AesCipher {
    pub key: AesKey,
}

fn normalize_key(key: &[u8]) -> Vec<u8> {
    let target = match key.len() {
        0..=16 => 16,
        17..=24 => 24,
        _ => 32,
    };
    let mut out = vec![0u8; target];
    let take = key.len().min(target);
    out[..take].copy_from_slice(&key[..take]);
    out
}

fn encrypt_with_len<C>(key: &[u8], iv: &[u8], input: &[u8]) -> Vec<u8>
where
    C: cipher::BlockCipher + cipher::KeyInit,
    Encryptor<C>: KeyIvInit + BlockEncryptMut,
{
    Encryptor::<C>::new_from_slices(key, iv)
        .expect("valid AES params")
        .encrypt_padded_vec_mut::<Pkcs7>(input)
}

fn decrypt_with_len<C>(key: &[u8], iv: &[u8], input: &[u8]) -> Vec<u8>
where
    C: cipher::BlockCipher + cipher::KeyInit,
    Decryptor<C>: KeyIvInit + BlockDecryptMut,
{
    Decryptor::<C>::new_from_slices(key, iv)
        .expect("valid AES params")
        .decrypt_padded_vec_mut::<Pkcs7>(input)
        .unwrap_or_default()
}

impl Cipher for AesCipher {
    fn name(&self) -> &'static str {
        "aes"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        match self.key.key_len {
            16 => encrypt_with_len::<Aes128>(&self.key.key_bytes, &self.key.iv, input),
            24 => encrypt_with_len::<Aes192>(&self.key.key_bytes, &self.key.iv, input),
            _ => encrypt_with_len::<Aes256>(&self.key.key_bytes, &self.key.iv, input),
        }
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        match self.key.key_len {
            16 => decrypt_with_len::<Aes128>(&self.key.key_bytes, &self.key.iv, input),
            24 => decrypt_with_len::<Aes192>(&self.key.key_bytes, &self.key.iv, input),
            _ => decrypt_with_len::<Aes256>(&self.key.key_bytes, &self.key.iv, input),
        }
    }
}

pub fn aes_encrypt(input: &[u8], key: &[u8]) -> Vec<u8> {
    AesCipher { key: AesKey::new(key) }.encrypt(input)
}

pub fn aes_decrypt(input: &[u8], key: &[u8]) -> Vec<u8> {
    AesCipher { key: AesKey::new(key) }.decrypt(input)
}
