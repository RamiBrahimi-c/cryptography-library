use crate::cipher_interface::Cipher;
use crate::common::utils::{matrix_inverse_mod, multiply_matrix, random_invertible_matrix, Matrix};

#[derive(Clone, Debug)]
pub struct HillKey {
    pub matrix: Matrix,
    pub inverse: Matrix,
    pub modulus: usize,
}

impl Default for HillKey {
    fn default() -> Self {
        let matrix = vec![vec![1, 2], vec![3, 5]];
        let inverse = matrix_inverse_mod(&matrix, 256).expect("invertible default hill key");
        Self {
            matrix,
            inverse,
            modulus: 256,
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct HillCipher {
    pub key: HillKey,
}

impl HillKey {
    pub fn from_matrix(matrix: Matrix, modulus: usize) -> Option<Self> {
        let inverse = matrix_inverse_mod(&matrix, modulus)?;
        Some(Self {
            matrix,
            inverse,
            modulus,
        })
    }

    pub fn random(size: usize, modulus: usize) -> Self {
        let matrix = random_invertible_matrix(size, modulus);
        let inverse = matrix_inverse_mod(&matrix, modulus).expect("generated invertible matrix");
        Self {
            matrix,
            inverse,
            modulus,
        }
    }
}

fn normalize_block(block: &[u8]) -> Matrix {
    block.iter().map(|b| vec![*b]).collect()
}

fn apply(matrix: &Matrix, input: &[u8], modulo: usize) -> Vec<u8> {
    let width = matrix.len();
    let mut out = Vec::with_capacity(input.len());
    for chunk in input.chunks(width) {
        let mut padded = chunk.to_vec();
        while padded.len() < width {
            padded.push(0);
        }
        let encrypted = multiply_matrix(matrix, &normalize_block(&padded), modulo);
        for row in encrypted {
            out.push(row[0]);
        }
    }
    out
}

impl Cipher for HillCipher {
    fn name(&self) -> &'static str {
        "hill"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        let mut payload = (input.len() as u32).to_be_bytes().to_vec();
        payload.extend_from_slice(input);
        apply(&self.key.matrix, &payload, self.key.modulus)
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        let plain = apply(&self.key.inverse, input, self.key.modulus);
        if plain.len() < 4 {
            return plain;
        }
        let expected = u32::from_be_bytes([plain[0], plain[1], plain[2], plain[3]]) as usize;
        plain[4..4 + expected.min(plain.len().saturating_sub(4))].to_vec()
    }
}
