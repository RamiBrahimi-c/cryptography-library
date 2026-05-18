use crate::cipher_interface::Cipher;
use crate::common::utils::{chunk_pad, strip_pkcs7, xor_repeating};
use super::aes_lib::{aes_cbc_decrypt_buffer, aes_cbc_encrypt_buffer, aes_init_ctx_iv, AesCtx};

#[derive(Clone, Debug)]
pub struct AesKey {
    pub key_len: usize,
    pub key_bytes: Vec<u8>,
    pub ctx: AesCtx,
}

impl AesKey {
    pub fn new(key: impl AsRef<[u8]>) -> Self {
        let key_bytes = key.as_ref().to_vec();
        let mut ctx = AesCtx::default();
        aes_init_ctx_iv(&mut ctx, &key_bytes, &[0u8; 16]);
        Self {
            key_len: key_bytes.len(),
            key_bytes,
            ctx,
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

impl Cipher for AesCipher {
    fn name(&self) -> &'static str {
        "aes"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        let mut out = xor_repeating(&chunk_pad(input, 16), &self.key.key_bytes);
        let mut ctx = self.key.ctx.clone();
        aes_cbc_encrypt_buffer(&mut ctx, &mut out);
        out
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        let mut out = input.to_vec();
        let mut ctx = self.key.ctx.clone();
        aes_cbc_decrypt_buffer(&mut ctx, &mut out);
        strip_pkcs7(xor_repeating(&out, &self.key.key_bytes))
    }
}

pub fn aes_encrypt(input: &[u8], key: &[u8]) -> Vec<u8> {
    AesCipher { key: AesKey::new(key) }.encrypt(input)
}

pub fn aes_decrypt(input: &[u8], key: &[u8]) -> Vec<u8> {
    AesCipher { key: AesKey::new(key) }.decrypt(input)
}
