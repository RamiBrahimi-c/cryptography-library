use md5::{Digest as Md5Digest, Md5};
use sha2::{Digest as ShaDigest, Sha256, Sha512};
use std::fs::File;
use std::io::{Read, Result};
use std::time::Instant;

pub fn md5_hash(data: &[u8]) -> [u8; 16] {
    let mut hasher = Md5::new();
    hasher.update(data);
    hasher.finalize().into()
}

pub fn sha256_hash(data: &[u8]) -> [u8; 32] {
    let mut hasher = Sha256::new();
    hasher.update(data);
    hasher.finalize().into()
}

pub fn sha512_hash(data: &[u8]) -> [u8; 64] {
    let mut hasher = Sha512::new();
    hasher.update(data);
    hasher.finalize().into()
}

fn read_file(path: &str) -> Result<Vec<u8>> {
    let mut file = File::open(path)?;
    let mut data = Vec::new();
    file.read_to_end(&mut data)?;
    Ok(data)
}

pub fn md5_file(filename: &str) -> Result<[u8; 16]> {
    Ok(md5_hash(&read_file(filename)?))
}

pub fn sha256_file(filename: &str) -> Result<[u8; 32]> {
    Ok(sha256_hash(&read_file(filename)?))
}

fn count_diff_bits(left: &[u8], right: &[u8]) -> u32 {
    left.iter().zip(right).map(|(a, b)| (a ^ b).count_ones()).sum()
}

pub fn avalanche_test<F>(input: &[u8], hash_fn: F, digest_len: usize) -> f64
where
    F: Fn(&[u8]) -> Vec<u8>,
{
    if input.is_empty() {
        return 0.0;
    }
    let baseline = hash_fn(input);
    let mut flipped = input.to_vec();
    flipped[0] ^= 0x01;
    let changed = hash_fn(&flipped);
    let diff = count_diff_bits(&baseline[..digest_len], &changed[..digest_len]) as f64;
    diff / (digest_len as f64 * 8.0)
}

pub fn bench_hash<F>(hash_fn: F, total_bytes: usize) -> f64
where
    F: Fn(&[u8]) -> Vec<u8>,
{
    let payload = vec![0xA5; total_bytes];
    let start = Instant::now();
    let _ = hash_fn(&payload);
    let elapsed = start.elapsed().as_secs_f64().max(1e-9);
    total_bytes as f64 / elapsed / (1024.0 * 1024.0)
}

pub fn sha256_vec(data: &[u8]) -> Vec<u8> {
    sha256_hash(data).to_vec()
}
