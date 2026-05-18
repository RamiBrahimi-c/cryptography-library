pub type Uchar = u8;

#[derive(Clone, Debug)]
pub struct CipherDescriptor {
    pub name: &'static str,
}

impl CipherDescriptor {
    pub const fn new(name: &'static str) -> Self {
        Self { name }
    }
}

pub trait Cipher {
    fn name(&self) -> &'static str;
    fn encrypt(&self, input: &[u8]) -> Vec<u8>;
    fn decrypt(&self, input: &[u8]) -> Vec<u8>;
}
