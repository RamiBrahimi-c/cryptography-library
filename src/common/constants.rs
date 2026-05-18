use crate::cipher_interface::Uchar;

pub const MATRIX_MAX_DEGREE: usize = 20;
pub const ARRAY_MAX_SIZE: usize = 256;
pub const RC4_KEY_MAX_SIZE: usize = 256;
pub const ALPHABET_LENGTH: usize = 128;
pub const SUBSTITUTION_ALPHABET_LENGTH: usize = 26;

pub fn ascii_alphabet() -> Vec<Uchar> {
    (0..ALPHABET_LENGTH as Uchar).collect()
}
