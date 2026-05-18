use crate::cipher_interface::Uchar;
use crate::common::constants::{ALPHABET_LENGTH, MATRIX_MAX_DEGREE};
use num_bigint::{BigInt, BigUint, RandBigInt, ToBigInt};
use num_traits::{One, Zero};
use rand::thread_rng;

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

pub fn biguint_gcd(mut a: BigUint, mut b: BigUint) -> BigUint {
    while !b.is_zero() {
        let t = a % &b;
        a = b;
        b = t;
    }
    a
}

pub fn biguint_mod_inverse(a: &BigUint, m: &BigUint) -> Option<BigUint> {
    fn egcd(a: BigInt, b: BigInt) -> (BigInt, BigInt, BigInt) {
        if b.is_zero() {
            (a, BigInt::one(), BigInt::zero())
        } else {
            let (g, x, y) = egcd(b.clone(), a.clone() % b.clone());
            (g, y.clone(), x - (a / b) * y)
        }
    }

    let (g, x, _) = egcd(a.to_bigint()?, m.to_bigint()?);
    if g != BigInt::one() {
        return None;
    }
    let modulus = m.to_bigint()?;
    ((x % &modulus + &modulus) % &modulus).try_into().ok()
}

pub fn is_probable_prime(n: &BigUint, rounds: u32) -> bool {
    let two = BigUint::from(2u32);
    let three = BigUint::from(3u32);
    if *n < two {
        return false;
    }
    if *n == two || *n == three {
        return true;
    }
    if n % &two == BigUint::zero() {
        return false;
    }

    let one = BigUint::one();
    let n_minus_one = n - &one;
    let mut d = n_minus_one.clone();
    let mut s = 0u32;
    while &d % &two == BigUint::zero() {
        d >>= 1;
        s += 1;
    }

    let mut rng = thread_rng();
    'outer: for _ in 0..rounds {
        let a = rng.gen_biguint_range(&two, &(n - &two));
        let mut x = a.modpow(&d, n);
        if x == one || x == n_minus_one {
            continue;
        }
        for _ in 1..s {
            x = x.modpow(&two, n);
            if x == n_minus_one {
                continue 'outer;
            }
        }
        return false;
    }
    true
}

pub fn generate_probable_prime(bits: usize) -> BigUint {
    let mut rng = thread_rng();
    let top_bit = BigUint::one() << bits.saturating_sub(1);
    loop {
        let mut candidate = rng.gen_biguint(bits as u64);
        candidate |= &top_bit;
        candidate |= BigUint::one();
        if is_probable_prime(&candidate, 8) {
            return candidate;
        }
    }
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

fn minor_matrix(matrix: &[Vec<i64>], skip_row: usize, skip_col: usize) -> Vec<Vec<i64>> {
    let mut out = Vec::with_capacity(matrix.len().saturating_sub(1));
    for (r, row) in matrix.iter().enumerate() {
        if r == skip_row {
            continue;
        }
        let mut out_row = Vec::with_capacity(row.len().saturating_sub(1));
        for (c, value) in row.iter().enumerate() {
            if c != skip_col {
                out_row.push(*value);
            }
        }
        out.push(out_row);
    }
    out
}

fn determinant_recursive(matrix: &[Vec<i64>], modulo: i64) -> i64 {
    match matrix.len() {
        0 => 0,
        1 => matrix[0][0].rem_euclid(modulo),
        2 => (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]).rem_euclid(modulo),
        _ => {
            let mut det = 0i64;
            for (col, value) in matrix[0].iter().enumerate() {
                let sign = if col % 2 == 0 { 1 } else { -1 };
                let sub = determinant_recursive(&minor_matrix(matrix, 0, col), modulo);
                det = (det + sign * value * sub).rem_euclid(modulo);
            }
            det.rem_euclid(modulo)
        }
    }
}

pub fn determinant_mod(matrix: &Matrix, modulo: usize) -> i64 {
    let as_i64: Vec<Vec<i64>> = matrix
        .iter()
        .map(|row| row.iter().map(|&v| v as i64).collect())
        .collect();
    determinant_recursive(&as_i64, modulo as i64)
}

pub fn matrix_inverse_mod(matrix: &Matrix, modulo: usize) -> Option<Matrix> {
    if matrix.is_empty() || matrix.len() != matrix[0].len() {
        return None;
    }
    let modulo = modulo as i64;
    let size = matrix.len();
    let as_i64: Vec<Vec<i64>> = matrix
        .iter()
        .map(|row| row.iter().map(|&v| v as i64).collect())
        .collect();
    let det = determinant_recursive(&as_i64, modulo);
    let det_inv = mod_inverse(det, modulo)?;
    let mut cofactors = vec![vec![0i64; size]; size];
    for row in 0..size {
        for col in 0..size {
            let sign = if (row + col) % 2 == 0 { 1 } else { -1 };
            let minor = determinant_recursive(&minor_matrix(&as_i64, row, col), modulo);
            cofactors[row][col] = (sign * minor).rem_euclid(modulo);
        }
    }

    let mut out = vec![vec![0u8; size]; size];
    for row in 0..size {
        for col in 0..size {
            let adj = cofactors[col][row];
            out[row][col] = (adj * det_inv).rem_euclid(modulo) as u8;
        }
    }
    Some(out)
}

pub fn random_invertible_matrix(size: usize, modulo: usize) -> Matrix {
    use rand::Rng;

    let mut rng = thread_rng();
    loop {
        let mut matrix = vec![vec![0u8; size]; size];
        for row in &mut matrix {
            for value in row {
                *value = rng.gen_range(0..modulo) as u8;
            }
        }
        if matrix_inverse_mod(&matrix, modulo).is_some() {
            return matrix;
        }
    }
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
    let rem = out.len() % block_size;
    let pad = if rem == 0 { block_size } else { block_size - rem };
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
