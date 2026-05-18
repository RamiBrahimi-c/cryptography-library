use crate::cipher_interface::Cipher;
use crate::common::utils::{matrix_inverse_mod, multiply_matrix, Matrix};

#[derive(Clone, Debug)]
pub struct HillKey {
    pub matrix: Matrix,
    pub inverse: Matrix,
}

impl Default for HillKey {
    fn default() -> Self {
        let matrix = vec![vec![3, 3], vec![2, 5]];
        let inverse = matrix_inverse_mod(&matrix, 26);
        Self { matrix, inverse }
    }
}

#[derive(Clone, Debug, Default)]
pub struct HillCipher {
    pub key: HillKey,
}

fn normalize_block(block: &[u8]) -> Matrix {
    block
        .iter()
        .map(|b| vec![b.to_ascii_uppercase().saturating_sub(b'A')])
        .collect()
}

fn apply(matrix: &Matrix, input: &[u8]) -> Vec<u8> {
    let width = matrix.len();
    let mut out = Vec::with_capacity(input.len());
    for chunk in input.chunks(width) {
        let mut padded = chunk.to_vec();
        while padded.len() < width {
            padded.push(b'X');
        }
        let encrypted = multiply_matrix(matrix, &normalize_block(&padded), 26);
        for row in encrypted {
            out.push(row[0] % 26 + b'A');
        }
    }
    out
}

impl Cipher for HillCipher {
    fn name(&self) -> &'static str {
        "hill"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        apply(&self.key.matrix, input)
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        apply(&self.key.inverse, input)
    }
}
