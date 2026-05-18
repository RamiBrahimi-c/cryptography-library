pub fn encrypt_fistel<F>(src: &[u8], rounds: usize, round_fn: F) -> Vec<u8>
where
    F: Fn(&[u8]) -> Vec<u8>,
{
    let split = src.len() / 2;
    let mut left = src[..split].to_vec();
    let mut right = src[split..].to_vec();
    for _ in 0..rounds {
        let mixed = round_fn(&right);
        let next_right: Vec<u8> = left
            .iter()
            .zip(mixed.iter().cycle())
            .map(|(a, b)| a ^ b)
            .collect();
        left = right;
        right = next_right;
    }
    [left, right].concat()
}
