#![allow(dead_code)]

pub mod cipher_interface;
pub mod ciphers;
pub mod common;

use cipher_interface::CipherDescriptor;

pub fn registered_ciphers() -> Vec<CipherDescriptor> {
    vec![
        CipherDescriptor::new("affine"),
        CipherDescriptor::new("caesar"),
        CipherDescriptor::new("hill"),
        CipherDescriptor::new("playfair"),
        CipherDescriptor::new("substitution"),
        CipherDescriptor::new("vigenere"),
        CipherDescriptor::new("aes"),
        CipherDescriptor::new("des"),
        CipherDescriptor::new("rc4"),
        CipherDescriptor::new("rsa"),
        CipherDescriptor::new("elgamal"),
        CipherDescriptor::new("diffie-hellman"),
        CipherDescriptor::new("hash"),
    ]
}
