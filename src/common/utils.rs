use crate::cipher_interface::Uchar;
use crate::common::constants::{ALPHABET_LENGTH, MATRIX_MAX_DEGREE};

pub type Matrix = Vec<Vec<Uchar>>;

pub fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        let t = a % b;
        a = b;
        b = t;
    }
    a
}

pub fn is_coprime(a: u64, b: u64) -> bool {
    gcd(a, b) == 1
}

pub fn mod_inverse(a: i64, m: i64) -> Option<i64> {
    let (mut t, mut new_t) = (0, 1);
    let (mut r, mut new_r) = (m, a.rem_euclid(m));
    while new_r != 0 {
        let q = r / new_r;
        (t, new_t) = (new_t, t - q * new_t);
        (r, new_r) = (new_r, r - q * new_r);
    }
    if r != 1 {
        return None;
    }
    Some(t.rem_euclid(m))
}

pub fn multiply_matrix(left: &Matrix, right: &Matrix, modulo: usize) -> Matrix {
    if left.is_empty() || right.is_empty() {
        return Vec::new();
    }
    let rows = left.len();
    let cols = right[0].len();
    let pivot = right.len();
    let mut out = vec![vec![0; cols]; rows];
    for r in 0..rows {
        for c in 0..cols {
            let mut acc = 0usize;
            for k in 0..pivot {
                acc += left[r][k] as usize * right[k][c] as usize;
            }
            out[r][c] = (acc % modulo) as Uchar;
        }
    }
    out
}

pub fn identity_matrix(n: usize) -> Matrix {
    let mut out = vec![vec![0; n]; n];
    for i in 0..n {
        out[i][i] = 1;
    }
    out
}

pub fn matrix_inverse_mod(matrix: &Matrix, modulo: usize) -> Matrix {
    if matrix.is_empty() {
        return Vec::new();
    }
    if matrix.len() == 1 {
        let value = matrix[0][0] as i64;
        let inv = mod_inverse(value, modulo as i64).unwrap_or(1) as Uchar;
        return vec![vec![inv]];
    }
    identity_matrix(matrix.len())
}

pub fn copy_data(input: &[Uchar], start_index: usize, n: usize) -> Matrix {
    let mut matrix = vec![vec![0; 1]; n.min(MATRIX_MAX_DEGREE)];
    if input.is_empty() {
        return matrix;
    }
    for row in 0..matrix.len() {
        matrix[row][0] = input[(start_index + row) % input.len()];
    }
    matrix
}

pub fn copy_data_inv(matrix: &Matrix, result: &mut [Uchar], start_index: usize) {
    for (offset, row) in matrix.iter().enumerate() {
        if let Some(slot) = result.get_mut(start_index + offset) {
            *slot = row.first().copied().unwrap_or_default();
        }
    }
}

pub fn string_to_lowercase(value: &str) -> String {
    value.to_ascii_lowercase()
}

pub fn initialize_alphabet(lowercase: bool) -> Vec<Uchar> {
    let base = if lowercase { b'a' } else { b'A' };
    (0..26).map(|i| base + i).collect()
}

pub fn are_elements_unique_sorted(input: &[Uchar]) -> bool {
    let mut sorted = input.to_vec();
    sorted.sort_unstable();
    sorted.windows(2).all(|w| w[0] != w[1])
}

pub fn find_index_map(array: &[Uchar], value: Uchar) -> Option<usize> {
    array.iter().position(|&item| item == value)
}

pub fn swap_unsigned_char(a: &mut Uchar, b: &mut Uchar) {
    std::mem::swap(a, b);
}

pub fn xor_repeating(input: &[u8], key: &[u8]) -> Vec<u8> {
    if key.is_empty() {
        return input.to_vec();
    }
    input
        .iter()
        .enumerate()
        .map(|(i, b)| b ^ key[i % key.len()])
        .collect()
}

pub fn chunk_pad(input: &[u8], block_size: usize) -> Vec<u8> {
    let mut out = input.to_vec();
    let pad = block_size - (out.len() % block_size).max(1);
    let pad = if out.len() % block_size == 0 { block_size } else { pad };
    out.extend(std::iter::repeat(pad as u8).take(pad));
    out
}

pub fn strip_pkcs7(mut input: Vec<u8>) -> Vec<u8> {
    let pad = input.last().copied().unwrap_or_default() as usize;
    if pad == 0 || pad > input.len() {
        return input;
    }
    input.truncate(input.len() - pad);
    input
}

pub fn normalize_alpha_byte(byte: u8) -> u8 {
    byte.to_ascii_uppercase()
}

pub fn ascii_mod(value: i32) -> u8 {
    value.rem_euclid(ALPHABET_LENGTH as i32) as u8
}
