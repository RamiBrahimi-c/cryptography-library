use crate::cipher_interface::Cipher;

#[derive(Clone, Debug)]
pub struct PlayfairCipher {
    pub key: String,
}

impl Default for PlayfairCipher {
    fn default() -> Self {
        Self {
            key: "MONARCHY".to_string(),
        }
    }
}

fn build_grid(key: &str) -> [[u8; 5]; 5] {
    let mut seen = [false; 26];
    seen[(b'J' - b'A') as usize] = true;
    let mut letters = Vec::with_capacity(25);
    for byte in key.bytes().chain(b"ABCDEFGHIJKLMNOPQRSTUVWXYZ".iter().copied()) {
        if !byte.is_ascii_alphabetic() {
            continue;
        }
        let upper = if byte.to_ascii_uppercase() == b'J' {
            b'I'
        } else {
            byte.to_ascii_uppercase()
        };
        let idx = (upper - b'A') as usize;
        if !seen[idx] {
            seen[idx] = true;
            letters.push(upper);
        }
    }
    let mut grid = [[b'A'; 5]; 5];
    for (i, letter) in letters.into_iter().enumerate().take(25) {
        grid[i / 5][i % 5] = letter;
    }
    grid
}

fn find_pos(grid: &[[u8; 5]; 5], target: u8) -> (usize, usize) {
    for (row, line) in grid.iter().enumerate() {
        for (col, &value) in line.iter().enumerate() {
            if value == target {
                return (row, col);
            }
        }
    }
    (0, 0)
}

fn normalize_letters(input: &[u8]) -> Vec<u8> {
    input
        .iter()
        .copied()
        .filter(|b| b.is_ascii_alphabetic())
        .map(|b| {
            let upper = b.to_ascii_uppercase();
            if upper == b'J' { b'I' } else { upper }
        })
        .collect()
}

fn digraphs_for_encrypt(input: &[u8]) -> Vec<(u8, u8)> {
    let clean = normalize_letters(input);
    let mut out = Vec::new();
    let mut i = 0;
    while i < clean.len() {
        let first = clean[i];
        let second = clean.get(i + 1).copied();
        match second {
            Some(next) if next != first => {
                out.push((first, next));
                i += 2;
            }
            Some(_) => {
                let filler = if first == b'X' { b'Q' } else { b'X' };
                out.push((first, filler));
                i += 1;
            }
            None => {
                let filler = if first == b'X' { b'Q' } else { b'X' };
                out.push((first, filler));
                i += 1;
            }
        }
    }
    out
}

fn digraphs_for_decrypt(input: &[u8]) -> Vec<(u8, u8)> {
    let clean = normalize_letters(input);
    clean
        .chunks(2)
        .map(|chunk| (chunk[0], *chunk.get(1).unwrap_or(&b'X')))
        .collect()
}

fn transform_pair(grid: &[[u8; 5]; 5], a: u8, b: u8, decrypt: bool) -> (u8, u8) {
    let (ra, ca) = find_pos(grid, a);
    let (rb, cb) = find_pos(grid, b);
    if ra == rb {
        let shift = if decrypt { 4 } else { 1 };
        (grid[ra][(ca + shift) % 5], grid[rb][(cb + shift) % 5])
    } else if ca == cb {
        let shift = if decrypt { 4 } else { 1 };
        (grid[(ra + shift) % 5][ca], grid[(rb + shift) % 5][cb])
    } else {
        (grid[ra][cb], grid[rb][ca])
    }
}

impl Cipher for PlayfairCipher {
    fn name(&self) -> &'static str {
        "playfair"
    }

    fn encrypt(&self, input: &[u8]) -> Vec<u8> {
        let grid = build_grid(&self.key);
        digraphs_for_encrypt(input)
            .into_iter()
            .flat_map(|(a, b)| {
                let (x, y) = transform_pair(&grid, a, b, false);
                [x, y]
            })
            .collect()
    }

    fn decrypt(&self, input: &[u8]) -> Vec<u8> {
        let grid = build_grid(&self.key);
        digraphs_for_decrypt(input)
            .into_iter()
            .flat_map(|(a, b)| {
                let (x, y) = transform_pair(&grid, a, b, true);
                [x, y]
            })
            .collect()
    }
}

pub fn encrypt_playfair(src: &[u8]) -> Vec<u8> {
    PlayfairCipher::default().encrypt(src)
}
