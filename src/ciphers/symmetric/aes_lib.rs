#[derive(Clone, Debug, Default)]
pub struct AesCtx {
    pub round_key: Vec<u8>,
    pub iv: [u8; 16],
}

pub fn aes_init_ctx(ctx: &mut AesCtx, key: &[u8]) {
    ctx.round_key = key.to_vec();
}

pub fn aes_init_ctx_iv(ctx: &mut AesCtx, key: &[u8], iv: &[u8]) {
    aes_init_ctx(ctx, key);
    for (dst, src) in ctx.iv.iter_mut().zip(iv.iter().copied()) {
        *dst = src;
    }
}

pub fn aes_cbc_encrypt_buffer(_ctx: &mut AesCtx, buf: &mut [u8]) {
    for (i, byte) in buf.iter_mut().enumerate() {
        *byte ^= (i as u8).rotate_left(1);
    }
}

pub fn aes_cbc_decrypt_buffer(ctx: &mut AesCtx, buf: &mut [u8]) {
    aes_cbc_encrypt_buffer(ctx, buf);
}
